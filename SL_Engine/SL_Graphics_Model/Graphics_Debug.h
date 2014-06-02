#ifndef SL_GRAPHICS_DEBUG_H_123
#define SL_GRAPHICS_DEBUG_H_123
#include "..\glew\include\GL\glew.h"
#include "..\glfw\include\GLFW\glfw3.h"
#include "..\SL_Utilities\Debug_Output.h"

#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)
// GL ERROR CHECK
inline int CheckGLError(char *file, int line)
{
	GLenum glErr=  glGetError();
	while (glErr != GL_NO_ERROR)
	{
		char* str1 = (char*)gluErrorString(glErr);
		if (str1) DEBUG_MSG("GL Error #%(%) in File at line: %", glErr, str1, file, line);
		else DEBUG_MSG("GL Error #% in File at line: %", glErr, file, line);
		glErr = glGetError();
	}
	return 0;
}

#endif