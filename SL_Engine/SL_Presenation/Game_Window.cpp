#include "stdafx.h"
#include "Game_Window.h"
#include "..\SL_Terrain_Graphics\Terrain_Graphics.h"
#include "..\SL_Utilities\Debug_Output.h"
#include "..\SL_Graphics\Text.h"

SL_Presentation::Game_Window::Game_Window(){
	DEBUG_MSG("Begin Game_Window");

	//Terrain = new SL_Terrain::Terrain_Graphics();
	
	//Add_Draw_CallBack(std::bind(&SL_Terrain::Terrain_Graphics::Draw, Terrain, std::placeholders::_1, std::placeholders::_2));
	_Text = new SL_Graphics::Text();
	DEBUG_MSG("End Game_Window");
}
SL_Presentation::Game_Window::~Game_Window(){
	//delete Terrain;
	delete _Text; 
}
void SL_Presentation::Game_Window::Run(){
	while (Begin()){
		Update();
		Draw();
		_Text->Draw();
		End();
	}
}