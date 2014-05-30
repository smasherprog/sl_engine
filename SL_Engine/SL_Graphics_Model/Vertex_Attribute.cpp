#include "stdafx.h"
#include "Vertex_Attribute.h"
//format is [elementtype]:[numelements][elementtype][normalized(optional)]

bool SL_Graphics::Model::Vector_Attribute::Parse(std::string format, SL_Graphics::Model::Vector_Attribute& attr){
	auto index = format.find_first_of(':');
	if (index != std::string::npos) attr.name = format.substr(0, index);
	else
	{
		DEBUG_MSG("Vertex attribute format not understood('%')", format);
		return false;
	}
	index += 1;
	if (index < format.size()) attr.size = format[index] - '0';
	else {
		DEBUG_MSG("Vertex attribute format not understood('%')", format);
		return false;
	}
	index += 1;
	char ctype;
	if (index < format.size()) ctype = format[index];
	else {
		DEBUG_MSG("Vertex attribute format not understood('%')", format);
		return false;
	}
	index += 1;
	if (index < format.size()) {
		auto t = format[index];
		if (t == 'n') attr.normalized = true;
	}

	switch (ctype)
	{
	case 'b': attr.type = GL_BYTE;           break;
	case 'B': attr.type = GL_UNSIGNED_BYTE;  break;
	case 's': attr.type = GL_SHORT;          break;
	case 'S': attr.type = GL_UNSIGNED_SHORT; break;
	case 'i': attr.type = GL_INT;            break;
	case 'I': attr.type = GL_UNSIGNED_INT;   break;
	case 'f': attr.type = GL_FLOAT;          break;
	default:  attr.type = 0;                 break;
	}


	return true;
}

void SL_Graphics::Model::Vector_Attribute::enable(){
	if (stride == 0) return;
	if (index == -1)
	{
		GLint program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		if (program == 0) return;
		index = glGetAttribLocation(program, name.c_str());
		if (index == -1) return;
	}
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, (GLvoid*)(offset));

}