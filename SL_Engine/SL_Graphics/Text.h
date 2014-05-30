#ifndef SL_TEXT_H
#define SL_TEXT_H
#include <string>
#include "..\SL_Font_Utilities\Font.h"
#include "Shader.h"
#include "..\SL_Graphics_Model\Vertex_Buffer.h"
#include "..\glm\glm.hpp"
#include "..\glm\ext.hpp"


namespace SL_Graphics{

	class Text{
	protected:
		SL_Graphics::Shader _Shader;
		SL_Graphics::Model::Vertex_Buffer buffer;
		SL_Font::Font_Wrapper _Font;
		std::string _Text;
		float _Font_Size;
		glm::vec2 _Pos;

	public:
		Text();
		~Text();
		float Get_Font_Size() const { return _Font_Size; }
		void Set_Font_Size(float size_in_pixels){ _Font_Size = size_in_pixels; }
		void Set_Text(std::string text, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
		void Set_Pos(glm::vec2 pos){ _Pos = pos; }
		glm::vec2 Get_Pos() const { return _Pos; }

		void Clear();
		void Draw();
	};

};

#endif
