#include "stdafx.h"
#include "Texture_Factory.h"
#include "Texture.h"
#include "..\SL_Utilities\Debug_Output.h"

std::map<std::string, SL_Graphics::Texture> SL_Graphics::Texture_Factory::_INTERNAL::_Texture_Cache;

std::shared_ptr<SL_Graphics::Texture> SL_Graphics::Texture_Factory::Load(const std::string& str){

	return nullptr;
}
void SL_Graphics::Texture_Factory::DeInit(){

}
void SL_Graphics::Texture_Factory::Init(){

}
void SL_Graphics::Texture_Factory::_INTERNAL::UnLoad(Texture* tex){


}