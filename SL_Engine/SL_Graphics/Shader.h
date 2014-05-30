#ifndef SHADER_H_123
#define SHADER_H_123
#include <vector>


namespace SL_Graphics{
	class Shader{
		unsigned int _Program;
		std::vector<unsigned int> _ShaderObjList;
	public:
		Shader();
		virtual ~Shader();
		void Enable();

		bool AddShader_FromText(unsigned int ShaderType, const char* f);
		bool AddShader_FromFile(unsigned int ShaderType, const char* f);
		bool Finalize();
		int GetUniformLocation(const char* pUniformName);
		int GetProgramParam(int param);
	};
};
#endif