#ifndef SHADER_H_123
#define SHADER_H_123
#include <vector>
#include <string>

namespace SL_Graphics{
	class Shader{
		unsigned int _Program;
		std::vector<unsigned int> _ShaderObjList;
		std::vector<std::string> _Active_Texture_Units;
		void _Parse(std::vector<char> file);
		bool _Parse_For_Texture(const std::string& line);

	public:
		Shader();
		virtual ~Shader();
		void Enable();

		bool AddShader_FromText(unsigned int ShaderType, const std::string f){ return AddShader_FromText(ShaderType, f.c_str()); }
		bool AddShader_FromText(unsigned int ShaderType, const char* f);
		bool AddShader_FromFile(unsigned int ShaderType, const std::string f){ return AddShader_FromFile(ShaderType, f.c_str()); }
		bool AddShader_FromFile(unsigned int ShaderType, const char* f);
		bool Finalize();
		int GetUniformLocation(const char* pUniformName);
		int GetProgramParam(int param);
	};
};
#endif