#include "stdafx.h"
#include "Font_Factory.h"
#include "..\freetype\include\ft2build.h"
#include FT_FREETYPE_H

#include <filesystem>
#include <fstream>
#include <string.h>
#include <algorithm>
#include "..\SL_Utilities\Timer.h"
#include "Font.h"
#include <future>
#include "..\SL_Utilities\Debug_Output.h"
#include "edtaa3func.h"

FT_Library SL_Font::_INTERNAL::_Font_Library = nullptr;
std::map<std::string, std::string> SL_Font::_INTERNAL::_Installed_Fonts;
std::map<std::string, std::weak_ptr<SL_Font::Font>> SL_Font::_INTERNAL::_Cached_Fonts;
std::string SL_Font::_INTERNAL::_PathToFonts;
std::mutex SL_Font::_INTERNAL::Mutex;

#define HRESf 64.f

bool SL_Font::Init(std::string pathtofonts){
	DEBUG_MSG("BEGIN: %", __FUNCTION__);
	if (FT_Init_FreeType(&SL_Font::_INTERNAL::_Font_Library)) {
		DEBUG_MSG("Could not init freetype library");
		return false;
	}
	SL_Font::_INTERNAL::_PathToFonts = pathtofonts;
	RefreshFonts();
	DEBUG_MSG("END: %", __FUNCTION__);
	return true;
}
struct font_builder_ret{
	std::string Font_Name;
	std::string Path_To_Font;
};
font_builder_ret Get_Font_Name(const std::string& filename);

void SL_Font::RefreshFonts(){
	SL_Utilities::Timer t(true);//start the timer

	std::vector<std::future<font_builder_ret>> fonts;

	for (auto it = std::tr2::sys::directory_iterator(SL_Font::_INTERNAL::_PathToFonts); it != std::tr2::sys::directory_iterator(); ++it)
	{
		auto s(it->path());
		fonts.push_back(std::async(std::launch::async, [=] {
			if (std::tr2::sys::extension(s) == ".ttf") return Get_Font_Name(s);
			return font_builder_ret();
		}));
	}

	int count = 0;
	std::lock_guard<std::mutex> lock(SL_Font::_INTERNAL::Mutex);
	for (auto &x : fonts){
		auto fs = x.get();
		if (fs.Font_Name.size() > 2){
			count += 1;
			SL_Font::_INTERNAL::_Installed_Fonts[fs.Font_Name] = fs.Path_To_Font;
			DEBUG_MSG(fs.Font_Name.c_str());
		}
	}
	t.Stop();
	DEBUG_MSG("Found % Fonts in %ms", count, t.Elapsed());
	
}

void SL_Font::_INTERNAL::Release_Font(SL_Font::Font* font){
	if (font == nullptr) return;
	std::lock_guard<std::mutex> lock(SL_Font::_INTERNAL::Mutex);
	auto f = SL_Font::_INTERNAL::_Cached_Fonts.find(font->Font_Name);
	if (f != SL_Font::_INTERNAL::_Cached_Fonts.end()) {
		glDeleteTextures(1, &font->Texture_ID);
		SL_Font::_INTERNAL::_Cached_Fonts.erase(f);
	}
	delete font;
}


// ------------------------------------------------------ make_distance_map ---
unsigned char *
make_distance_map(unsigned char *img,
unsigned int width, unsigned int height)
{
	short * xdist = (short *)malloc(width * height * sizeof(short));
	short * ydist = (short *)malloc(width * height * sizeof(short));
	double * gx = (double *)calloc(width * height, sizeof(double));
	double * gy = (double *)calloc(width * height, sizeof(double));
	double * data = (double *)calloc(width * height, sizeof(double));
	double * outside = (double *)calloc(width * height, sizeof(double));
	double * inside = (double *)calloc(width * height, sizeof(double));
	int i;

	// Convert img into double (data)
	double img_min = 255, img_max = -255;
	for (i = 0; i<width*height; ++i)
	{
		double v = img[i];
		data[i] = v;
		if (v > img_max) img_max = v;
		if (v < img_min) img_min = v;
	}
	// Rescale image levels between 0 and 1
	for (i = 0; i<width*height; ++i)
	{
		data[i] = (img[i] - img_min) / img_max;
	}

	// Compute outside = edtaa3(bitmap); % Transform background (0's)
	computegradient(data, height, width, gx, gy);
	edtaa3(data, gx, gy, height, width, xdist, ydist, outside);
	for (i = 0; i<width*height; ++i)
		if (outside[i] < 0)
			outside[i] = 0.0;

	// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
	memset(gx, 0, sizeof(double)*width*height);
	memset(gy, 0, sizeof(double)*width*height);
	for (i = 0; i<width*height; ++i)
		data[i] = 1 - data[i];
	computegradient(data, height, width, gx, gy);
	edtaa3(data, gx, gy, height, width, xdist, ydist, inside);
	for (i = 0; i<width*height; ++i)
		if (inside[i] < 0)
			inside[i] = 0.0;

	// distmap = outside - inside; % Bipolar distance field
	unsigned char *out = (unsigned char *)malloc(width * height * sizeof(unsigned char));
	for (i = 0; i<width*height; ++i)
	{
		outside[i] -= inside[i];
		outside[i] = 128 + outside[i] * 16;
		if (outside[i] < 0) outside[i] = 0;
		if (outside[i] > 255) outside[i] = 255;
		out[i] = 255 - (unsigned char)outside[i];
		//out[i] = (unsigned char) outside[i];
	}

	free(xdist);
	free(ydist);
	free(gx);
	free(gy);
	free(data);
	free(outside);
	free(inside);
	return out;
}


char keys[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~;";
void Get_Kerning(char prev_glyph, SL_Font::Glyph& current_glyph, FT_Face face){
	auto glyph_index = FT_Get_Char_Index(face, current_glyph.code);
	FT_Vector kerning;
	for (auto c : keys){
		auto prev_index = FT_Get_Char_Index(face, prev_glyph);
		FT_Get_Kerning(face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning);
		current_glyph.Kerning[prev_glyph] = kerning.x / (float)(HRESf*HRESf);
	}
}


SL_Font::Font_Wrapper SL_Font::Create_Font(std::string font_name, int fontsize){
	std::lock_guard<std::mutex> lock(SL_Font::_INTERNAL::Mutex);
	auto font = SL_Font::_INTERNAL::_Installed_Fonts.find(font_name);
	if (font == SL_Font::_INTERNAL::_Installed_Fonts.end()) return Font_Wrapper();// font doesnt exist

	auto cachedfont = SL_Font::_INTERNAL::_Cached_Fonts.find(font_name);
	if (cachedfont != SL_Font::_INTERNAL::_Cached_Fonts.end()) {//fond might exist and be cached.. check
		auto sfont = Font_Wrapper(cachedfont->second.lock());
		if (sfont.Get_Font()) return sfont;
		else SL_Font::_INTERNAL::_Cached_Fonts.erase(cachedfont);//remove the font from the cached fonts 
	}
	fontsize = fontsize < 8 ? 8 : fontsize;// clamp the min value

	FT_Face face;
	if (FT_New_Face(SL_Font::_INTERNAL::_Font_Library, font->second.c_str(), 0, &face)) {
		DEBUG_MSG("Could not load font the font: '%' from path: '%'", font_name, font->second);
		return Font_Wrapper();
	}
	FT_Set_Pixel_Sizes(face, 0, fontsize);

	std::shared_ptr<Font> fontthingy(new Font(), _INTERNAL::Release_Font);
	fontthingy->Font_Name = font_name;
	fontthingy->Font_File = font->second;

	std::vector<std::vector<unsigned char>> bitmaps;
	bitmaps.resize(sizeof(fontthingy->Glyphs) / sizeof(Glyph));
	auto prev_char = ' ';
	auto count = 0;
	for (auto c : keys){
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			DEBUG_MSG("Could not load character '%'", c);
		}
		else {
			FT_GlyphSlot g = face->glyph;
		
			fontthingy->Glyphs[c].advance_x = g->advance.x / 64;
			fontthingy->Glyphs[c].advance_y = g->advance.y / 64;
			fontthingy->Glyphs[c].offset_x = g->bitmap_left;
			fontthingy->Glyphs[c].offset_y = g->bitmap_top;
			fontthingy->Glyphs[c].code = c;
			fontthingy->Glyphs[c].height = g->bitmap.rows;
			fontthingy->Glyphs[c].width = g->bitmap.width;
			bitmaps[c].resize(g->bitmap.pitch * g->bitmap.rows);
			auto p = bitmaps[c].data();
			memcpy(p, g->bitmap.buffer, g->bitmap.pitch * g->bitmap.rows);
			Get_Kerning(prev_char, fontthingy->Glyphs[c], face);
		}
	}
	int maxwidth(512), maxheight(512), curwidth(1), maxcharheight(1);

	for (auto x : fontthingy->Glyphs){
		maxcharheight = maxcharheight < x.height ? (x.height +1) : maxcharheight;
	}

	unsigned char* img = (unsigned char*)calloc(maxheight * maxwidth, sizeof(unsigned char));

	int w(1), h(1);
	for (auto& x : fontthingy->Glyphs){
		if (x.code == 0) continue;
		if (w + x.width >= maxwidth) {
			w = 1;
			h += maxcharheight +1;
		}
		x.s0 = (float)w / (float)maxwidth;
		x.t0 = (float)h / (float)maxheight;

		x.s1 = (float)(w + x.width)/ (float)maxwidth;
		x.t1 = (float)(h + x.height)/ (float)maxheight;

		for (int row = 0; row < x.height; row++){
			for (int c = 0; c < x.width; c++){
				img[(row + h) * maxwidth + c + w] = bitmaps[x.code][row* x.width + c];
			}
		}
		w += x.width+1;
	}

	unsigned char *map = make_distance_map(img, maxwidth, maxheight);

	glGenTextures(1, &fontthingy->Texture_ID);
	glBindTexture(GL_TEXTURE_2D, fontthingy->Texture_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, maxwidth, maxheight, 0, GL_RED, GL_UNSIGNED_BYTE, map);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(img);
	free(map);
	FT_Done_Face(face);
	SL_Font::_INTERNAL::_Cached_Fonts.insert(std::pair <std::string, std::weak_ptr<Font>>(font_name, fontthingy));
	return Font_Wrapper(fontthingy);
}


typedef struct _tagTT_OFFSET_TABLE{
	unsigned short	uMajorVersion;
	unsigned short	uMinorVersion;
	unsigned short	uNumOfTables;
	unsigned short	uSearchRange;
	unsigned short	uEntrySelector;
	unsigned short	uRangeShift;
}TT_OFFSET_TABLE;

typedef struct _tagTT_TABLE_DIRECTORY{
	char	szTag[4];			//table name
	unsigned long	uCheckSum;			//Check sum
	unsigned long	uOffset;			//Offset from beginning of file
	unsigned long	uLength;			//length of the table in bytes
}TT_TABLE_DIRECTORY;

typedef struct _tagTT_NAME_TABLE_HEADER{
	unsigned short	uFSelector;			//format selector. Always 0
	unsigned short	uNRCount;			//Name Records count
	unsigned short	uStorageOffset;		//Offset for strings storage, from start of the table
}TT_NAME_TABLE_HEADER;

typedef struct _tagTT_NAME_RECORD{
	unsigned short	uPlatformID;
	unsigned short	uEncodingID;
	unsigned short	uLanguageID;
	unsigned short	uNameID;
	unsigned short	uStringLength;
	unsigned short	uStringOffset;	//from start of storage area
}TT_NAME_RECORD;

#ifndef MAKEWORD
	#define MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#endif
#ifndef MAKELONG
#define MAKELONG(a, b)      ((long)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned long)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#endif
#ifndef LOBYTE
#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#endif
#ifndef HIBYTE
#define HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))
#endif
#ifndef LOWORD
#define LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#endif
#ifndef HIWORD
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#endif
#ifndef SWAPWORD
#define SWAPWORD(x)		MAKEWORD(HIBYTE(x), LOBYTE(x))
#endif
#ifndef SWAPLONG
#define SWAPLONG(x)		MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))
#endif

bool isEqual_CaseInsensitive(const std::string& a, const std::string& b)
{
	return a.size() == b.size() &&
		std::equal(a.begin(), a.end(), b.begin(), [](char cA, char cB) {
		return toupper(cA) == toupper(cB);
	});
}

font_builder_ret Get_Font_Name(const std::string& filename){
	font_builder_ret ret;
	ret.Path_To_Font = filename;
	std::ifstream f(filename);

	TT_OFFSET_TABLE ttOffsetTable;
	f.read((char*)&ttOffsetTable, sizeof(TT_OFFSET_TABLE));
	ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
	ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
	ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

	//check is this is a true type font and the version is 1.0
	if (ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0) return ret;

	TT_TABLE_DIRECTORY tblDir;
	bool bFound = false;
	std::string csTemp;

	for (int i = 0; i < ttOffsetTable.uNumOfTables; i++){
		f.read((char*)&tblDir, sizeof(TT_TABLE_DIRECTORY));
		csTemp = std::string(tblDir.szTag, 4);

		if (isEqual_CaseInsensitive(csTemp, "name")){
			bFound = true;
			tblDir.uLength = SWAPLONG(tblDir.uLength);
			tblDir.uOffset = SWAPLONG(tblDir.uOffset);
			break;
		}
	}
	if (bFound){
		f.seekg(tblDir.uOffset, std::ios_base::beg);

		TT_NAME_TABLE_HEADER ttNTHeader;
		f.read((char*)&ttNTHeader, sizeof(TT_NAME_TABLE_HEADER));
		ttNTHeader.uNRCount = SWAPWORD(ttNTHeader.uNRCount);
		ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);
		TT_NAME_RECORD ttRecord;

		for (int i = 0; i < ttNTHeader.uNRCount; i++){
			f.read((char*)&ttRecord, sizeof(TT_NAME_RECORD));
			ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);
			ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
			ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);

			if (ttRecord.uNameID == 4 && ttRecord.uStringLength>0){

				auto nPos = f.tellg();
				f.seekg(tblDir.uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset, std::ios_base::beg);
				std::vector<char> buffer;
				buffer.resize(ttRecord.uStringLength);
				f.read(&buffer[0], ttRecord.uStringLength);
				std::string something(&buffer[0], buffer.size());
				auto b = std::find_if(buffer.begin(), buffer.end(), [](char c){return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); });
				if (b != buffer.end()){
					for (auto x : buffer) if (x != 0) ret.Font_Name += x;
					return ret;
				}
				f.seekg(nPos, std::ios_base::beg);
			}
		}

	}
	return ret;
}
