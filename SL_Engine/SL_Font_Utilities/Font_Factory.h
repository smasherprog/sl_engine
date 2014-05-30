#ifndef SL_FONT_FACTORY_H_123
#define SL_FONT_FACTORY_H_123
#include "Font.h"
#include <string>
#include <memory>
#include <map>
#include <mutex>

typedef struct FT_LibraryRec_  *FT_Library;
namespace SL_Font{

	namespace _INTERNAL{
		extern FT_Library _Font_Library;
		extern std::map<std::string, std::string> _Installed_Fonts;
		extern std::map<std::string, std::weak_ptr<SL_Font::Font>> _Cached_Fonts;
		extern std::string _PathToFonts;
		extern std::mutex Mutex;
		void Release_Font(SL_Font::Font* font);
	};

	bool Init(std::string pathtofonts);
	void RefreshFonts();
	
	inline const std::map<std::string, std::string>& Get_Fonts() { return _INTERNAL::_Installed_Fonts; }

	SL_Font::Font_Wrapper Create_Font(std::string font_name, int fontsize);
};

#endif
