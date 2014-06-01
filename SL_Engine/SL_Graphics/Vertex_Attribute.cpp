#include "stdafx.h"
#include "Vertex_Attribute.h"

void SL_Graphics::Model::Vertex_Attribute::enable(){
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