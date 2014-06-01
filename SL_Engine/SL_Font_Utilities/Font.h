#ifndef SL_FONT_H_123
#define SL_FONT_H_123
#include <vector>
#include <string>
#include <memory>

namespace SL_Font{	
	/*
	* Glyph metrics:
	* --------------
	*
	*                       xmin                     xmax
	*                        |                         |
	*                        |<-------- width -------->|
	*                        |                         |
	*              |         +-------------------------+----------------- ymax
	*              |         |    ggggggggg   ggggg    |     ^        ^
	*              |         |   g:::::::::ggg::::g    |     |        |
	*              |         |  g:::::::::::::::::g    |     |        |
	*              |         | g::::::ggggg::::::gg    |     |        |
	*              |         | g:::::g     g:::::g     |     |        |
	*    offset_x -|-------->| g:::::g     g:::::g     |  offset_y    |
	*              |         | g:::::g     g:::::g     |     |        |
	*              |         | g::::::g    g:::::g     |     |        |
	*              |         | g:::::::ggggg:::::g     |     |        |
	*              |         |  g::::::::::::::::g     |     |      height
	*              |         |   gg::::::::::::::g     |     |        |
	*  baseline ---*---------|---- gggggggg::::::g-----*--------      |
	*            / |         |             g:::::g     |              |
	*     origin   |         | gggggg      g:::::g     |              |
	*              |         | g:::::gg   gg:::::g     |              |
	*              |         |  g::::::ggg:::::::g     |              |
	*              |         |   gg:::::::::::::g      |              |
	*              |         |     ggg::::::ggg        |              |
	*              |         |         gggggg          |              v
	*              |         +-------------------------+----------------- ymin
	*              |                                   |
	*              |------------- advance_x ---------->|
	*/

	/**
	* A structure that describe a glyph.
	*/
	class Glyph{
	public:
		Glyph() { Kerning.resize(128); }
		char code;  // ascii value of the character
		/**
		* Glyph's width in pixels.
		*/
		size_t width;

		/**
		* Glyph's height in pixels.
		*/
		size_t height;

		/**
		* Glyph's left bearing expressed in integer pixels.
		*/
		int offset_x;

		/**
		* Glyphs's top bearing expressed in integer pixels.
		*
		* Remember that this is the distance from the baseline to the top-most
		* glyph scanline, upwards y coordinates being positive.
		*/
		int offset_y;

		/**
		* For horizontal text layouts, this is the horizontal distance (in
		* fractional pixels) used to increment the pen position when the glyph is
		* drawn as part of a string of text.
		*/
		float advance_x;

		/**
		* For vertical text layouts, this is the vertical distance (in fractional
		* pixels) used to increment the pen position when the glyph is drawn as
		* part of a string of text.
		*/
		float advance_y;
		/**
		* First normalized texture coordinate (x) of top-left corner
		*/
		float s0;

		/**
		* Second normalized texture coordinate (y) of top-left corner
		*/
		float t0;

		/**
		* First normalized texture coordinate (x) of bottom-right corner
		*/
		float s1;

		/**
		* Second normalized texture coordinate (y) of bottom-right corner
		*/
		float t1;

		std::vector<float> Kerning;
	};
	class Font{
	public:
		Glyph Glyphs[128];//ascii defines up to 128 characters
		std::string Font_Name;
		std::string Font_File;
		unsigned int Texture_ID = 0;
	};
	class Font_Wrapper{
	public:
		Font_Wrapper() {}
		std::shared_ptr<Font> _Font;
		Font_Wrapper(std::shared_ptr<SL_Font::Font>& f) : _Font(f){}
		const SL_Font::Font* Get_Font() const { return _Font.get(); }
	};
};

#endif