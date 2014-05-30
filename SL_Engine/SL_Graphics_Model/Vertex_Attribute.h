#ifndef SL_VERTEX_ATTRIBUTE_H_123_H
#define SL_VERTEX_ATTRIBUTE_H_123_H
#include <string>
#include "..\glew\include\GL\glew.h"
#include "..\glfw\include\GLFW\glfw3.h"

namespace SL_Graphics{
	namespace Model{
#define MAX_VERTEX_ATTRIBUTE 16
		struct Vector_Attribute{
			std::string name = "default";
			GLuint index = -1;
			GLint size = 4;
			GLenum type = GL_FLOAT;
			GLboolean normalized = false;
			GLsizei stride = 0;
			GLsizei offset = 0;

			void enable();

			static bool Parse(std::string format, Vector_Attribute& attr);
		};
	};
};

#endif