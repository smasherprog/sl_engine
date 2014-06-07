#ifndef SHADER_H_123
#define SHADER_H_123
#include <vector>
#include <string>
#include "..\SL_Graphics_Model\BaseShader.h"

namespace SL_Graphics{
	class Shader: public SL_Graphics_Model::BaseShader{

		unsigned int _Program;
		std::vector<unsigned int> _ShaderObjList;

	public:
		Shader();
		virtual ~Shader();

		void Enable();
		virtual bool AddShader_FromText(unsigned int ShaderType, const char* f);
		bool Finalize();
		int GetUniformLocation(const char* pUniformName);
		int GetProgramParam(int param);
	};
};
#endif