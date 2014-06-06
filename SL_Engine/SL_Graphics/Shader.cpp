#include "stdafx.h"
#include "Shader.h"
#include "..\SL_Graphics_Model\Graphics_Debug.h"
#include <fstream>
#include <vector>
#include "..\SL_Utilities\String.h"
#include <algorithm>


SL_Graphics::Shader::Shader(){

}
SL_Graphics::Shader::~Shader(){
	for (auto x : _ShaderObjList) glDeleteShader(x);
	if (_Program != 0) glDeleteProgram(_Program);
}

bool SL_Graphics::Shader::AddShader_FromFile(unsigned int ShaderType, const char* f){
	std::ifstream file(f);
	if (file){
		std::streampos begin, end;
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();
		auto length = (unsigned int)(end - begin);
		std::vector<char> text(length+1);
		file.seekg(0, std::ios::beg);
		file.read(&text[0], length);
		text[length] = 0;//make sure it is null terminated
		auto b = AddShader_FromText(ShaderType, &text[0]);
		if(b) _Parse(text);
		return b;
	}
	return false;
}
bool SL_Graphics::Shader::AddShader_FromText(unsigned int ShaderType, const char* f){
	if (_Program == 0) _Program = glCreateProgram();
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (ShaderObj == 0) {
		DEBUG_MSG("Error creating shader type %", ShaderType);
		return false;
	}
	_ShaderObjList.push_back(ShaderObj);

	GLchar const* files[] = { f };
	GLint lengths[] = { 0 };

	glShaderSource(ShaderObj, 1, files, lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		DEBUG_MSG("Error compiling %\n%", ShaderType, InfoLog);
		return false;
	}
	glAttachShader(_Program, ShaderObj);
	
	return true;
}
bool SL_Graphics::Shader::Finalize(){

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(_Program);

	glGetProgramiv(_Program, GL_LINK_STATUS, &Success);

	if (Success == GL_FALSE) {
		glGetProgramInfoLog(_Program, sizeof(ErrorLog), NULL, ErrorLog);
		DEBUG_MSG("Error linking shader program: %", ErrorLog);
		return false;
	}

	glValidateProgram(_Program);
	glGetProgramiv(_Program, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(_Program, sizeof(ErrorLog), NULL, ErrorLog);
		DEBUG_MSG("Invalid shader program: %", ErrorLog);
		return false;
	}	
	// Delete the intermediate shader objects that have been added to the program
	for (auto it = _ShaderObjList.begin(); it != _ShaderObjList.end(); it++) glDeleteShader(*it);
	_ShaderObjList.clear();
	return true;
}
int SL_Graphics::Shader::GetUniformLocation(const char* pUniformName){
	GLuint Location = glGetUniformLocation(_Program, pUniformName);
	if (Location == -1) DEBUG_MSG("Warning! Unable to get the location of uniform %", pUniformName);
	return Location;
}
int SL_Graphics::Shader::GetProgramParam(int param){
	GLint ret;
	glGetProgramiv(_Program, param, &ret);
	return ret; 
}
void SL_Graphics::Shader::Enable(){
	glUseProgram(_Program);
	glActiveTexture(GL_TEXTURE0);
}
//simple parser right now. Basic checks. 
std::string remove_comments_and_linebreaks(std::string s){
	std::string lines;
	lines.reserve(s.size());
	int incommentblock = 0;
	//for each line
	for (const auto& line : split(s, '\n')){
		auto temp = trim(line);//trim any extra spaces off the begin and end of each line
		if (starts_with(temp, "/*")){ incommentblock += 1; continue; }//entered comment block skip
		else if (ends_with(temp, "*/")) { incommentblock -= 1; continue; }//finnaly, the comment block has decreased by one
		else if (incommentblock > 0) continue;//still inside of a comment block.. keep going
		else if (starts_with(temp, "//"))continue; //single line comment.. skip
		lines.append(temp);
	}
	return lines;
}
//this removes all spaces except single spaces. So, the text "hey  there  this" becomes  "hey there this"
std::string remove_spaces(const std::string& s){
	std::vector<char> otemp;
	otemp.reserve(s.size());

	bool prevspace = false;
	for (auto& x : s){
		if (x == ' '){
			if (prevspace)continue;
			else otemp.push_back(' ');
			prevspace = true;
		}
		else {
			otemp.push_back(x);
			prevspace = false;
		}
	}
	return std::string(&otemp[0], otemp.size());
}
void SL_Graphics::Shader::_Parse(std::vector<char> file){
	auto f = remove_comments_and_linebreaks(std::string(&file[0], file.size()));
	//for each semicolumn
	for (const auto& line : split(f, ';')){
		//std::cout << "LINE" << std::endl;
		//std::cout << line << std::endl;
		//now, restrict the max number of spaces to be 1 at a time
		auto cleanedline = remove_spaces(line);
		//parse for textures
		_Parse_For_Texture(cleanedline);
		//next parse for regular uniform variables
		//prase for the vertex layout
	}
}

bool SL_Graphics::Shader::_Parse_For_Texture(const std::string& line){
	const std::string texture("uniform sampler2D");
	auto it = line.find(texture);
	if (it != std::string::npos){//found texture 
		auto found = split(line.substr(it + texture.size(), line.size() - it), ' ');//split string where there are spaces to get the names of the textures
		std::cout << "Textures found: " << std::endl;
		for (auto& tex : found){
			if (!is_empty_or_whitespace(tex)) {
				std::cout << tex << std::endl;
				_Active_Texture_Units.push_back(tex);
			}
		}
		return true;
	}
	return false;
}
