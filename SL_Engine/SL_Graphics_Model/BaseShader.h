#ifndef SL_BASESHADER_H_123
#define SL_BASESHADER_H_123
#include <vector>
#include <string>

namespace SL_Graphics_Model{
	class BaseShader{	

	private:
		std::vector<std::string> _Active_Texture_Units;
	public:
		BaseShader();
		virtual ~BaseShader();

		bool AddShader_FromText(unsigned int ShaderType, const std::string f){ return AddShader_FromText(ShaderType, f.c_str()); }
		virtual bool AddShader_FromText(unsigned int ShaderType, const char* f) = 0;
		bool AddShader_FromFile(unsigned int ShaderType, const std::string f){ return AddShader_FromFile(ShaderType, f.c_str()); }
		bool AddShader_FromFile(unsigned int ShaderType, const char* f);

		friend class Shader_Parser;
	};
}

#endif
