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