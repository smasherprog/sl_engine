#ifndef SL_IVERTEX_ATTRIBUTE_H_123_H
#define SL_IVERTEX_ATTRIBUTE_H_123_H
#include <string>
#include "..\glew\include\GL\glew.h"
#include "..\glfw\include\GLFW\glfw3.h"

namespace SL_Graphics{
	namespace Model{
#define MAX_VERTEX_ATTRIBUTE 16
		class IVertex_Attribute{
		public:
			IVertex_Attribute(){}
			virtual ~IVertex_Attribute(){}

			std::string name = "default";
			GLuint index = -1;
			GLint size = 4;
			GLenum type = GL_FLOAT;
			GLboolean normalized = false;
			GLsizei stride = 0;
			GLsizei offset = 0;

			virtual void enable() = 0;

			static bool Parse(std::string format, IVertex_Attribute* attr);
		};
	};
};

#endif