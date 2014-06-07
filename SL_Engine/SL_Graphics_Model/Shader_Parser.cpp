#include "stdafx.h"
#include "Shader_Parser.h"
#include "assert.h"
#include "..\SL_Utilities\String.h"
#include "BaseShader.h"

SL_Graphics_Model::Shader_Parser::Shader_Parser(){

}
SL_Graphics_Model::Shader_Parser::~Shader_Parser(){

}

void SL_Graphics_Model::Shader_Parser::Run(void* obj, const char* text, size_t len){
	assert(obj != nullptr);
	Shader = (BaseShader*)obj;
	_Cleaned_Object = _remove_comments_and_linebreaks(std::string(text, len));
}

void SL_Graphics_Model::Shader_Parser::_Run(){
	//for each semicolumn
	for (const auto& line : split(_Cleaned_Object, ';')){
		//std::cout << "LINE" << std::endl;
		//std::cout << line << std::endl;
		//now, restrict the max number of spaces to be 1 at a time
		auto cleanedline = _remove_extraspaces(line);
		_Parse_For_Texture(cleanedline);//parse for textures
		//next parse for regular uniform variables
		//prase for the vertex layout
	}
}

bool SL_Graphics_Model::Shader_Parser::_Parse_For_Texture(const std::string& line){
	const std::string texture("uniform sampler2D");
	auto it = line.find(texture);
	if (it != std::string::npos){//found texture 
		auto found = split(line.substr(it + texture.size(), line.size() - it), ' ');//split string where there are spaces to get the names of the textures
		std::cout << "Textures found: " << std::endl;
		for (auto& tex : found){
			if (!is_empty_or_whitespace(tex)) {
				std::cout << tex << std::endl;
				Shader->_Active_Texture_Units.push_back(tex);
			}
		}
		return true;
	}
	return false;
}
