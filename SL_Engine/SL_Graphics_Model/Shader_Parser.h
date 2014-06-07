#ifndef SL_SHADER_PARSER_H_123
#define SL_SHADER_PARSER_H_123
#include "..\SL_Utilities\IParse.h"
#include <vector>
#include <string>

namespace SL_Graphics_Model{
	class BaseShader;
	class Shader_Parser : public SL_Utilities::IParse{

		void _Parse(std::vector<char> file);
		bool _Parse_For_Texture(const std::string& line);
		void _Run();
		BaseShader *Shader;
		std::string _Cleaned_Object;

	public:
		Shader_Parser();
		virtual ~Shader_Parser();

		virtual void Run(void* obj, const char* text, size_t len) override;


	};

}


#endif 


