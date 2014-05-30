#ifndef GAME_WINDOW_H_123
#define GAME_WINDOW_H_123
#include "..\SL_Graphics\Window_Base.h"

namespace SL_Terrain{
	class Terrain_Graphics;
	
};
namespace SL_Graphics{
	class Text;
};
namespace SL_Presentation{
	class Game_Window : public SL_Graphics::Window_Base{
	public:
	
		Game_Window();
		virtual ~Game_Window();
		void Run();
		SL_Terrain::Terrain_Graphics* Terrain;
		SL_Graphics::Text* _Text;
	};

};

#endif