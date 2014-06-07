#include "stdafx.h"
#include "BaseShader.h"
#include <fstream>
#include <memory>
#include "Shader_Parser.h"

SL_Graphics_Model::BaseShader::BaseShader(){

}
SL_Graphics_Model::BaseShader::~BaseShader(){

}

bool SL_Graphics_Model::BaseShader::AddShader_FromFile(unsigned int ShaderType, const char* f){
	std::ifstream file(f);
	if (file){
		std::streampos begin, end;
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();
		auto length = (unsigned int)(end - begin);
		std::vector<char> text(length + 1);
		file.seekg(0, std::ios::beg);
		file.read(&text[0], length);
		text[length] = 0;//make sure it is null terminated
		auto b = AddShader_FromText(ShaderType, &text[0]);
		if (b) {
			auto temp = std::make_unique<Shader_Parser>();
			temp->Run(this, &text[0], text.size());
		}
		return b;
	}
	return false;
}