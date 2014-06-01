#include "stdafx.h"
#include "Text.h"
#include "..\SL_Font_Utilities\Font_Factory.h"
#include "Vertex_Buffer_Factory.h"
#include "..\glm\glm.hpp"
#include "..\glm\ext.hpp"
#include "..\SL_Utilities\Debug_Output.h"
#include <string.h>

typedef struct {
	float x, y, z;    // position
	float s, t;       // texture
	float r, g, b, a; // color
} vertex_t;
#define DEFAULT_FONT_BUILD_SIZE 48.0f

void add_text(SL_Graphics::Model::IVertex_Buffer* buffer, SL_Font::Font_Wrapper& font,
const char * text, glm::vec4 * color, glm::vec2 * pen)
{
	size_t i;
	float r = color->r, g = color->g, b = color->b, a = color->a;
	auto glyphs = font.Get_Font()->Glyphs;
	for (i = 0; i<strlen(text); ++i)
	{
		auto& glyph = glyphs[text[i]];

		int kerning = 0;
		if (i > 0) kerning = glyph.Kerning[i - 1];
		
		pen->x += kerning;
		int x0 = (int)(pen->x + glyph.offset_x);
		int y0 = (int)(pen->y + glyph.offset_y);
		int x1 = (int)(x0 + glyph.width);
		int y1 = (int)(y0 - glyph.height);
		float s0 = glyph.s0;
		float t0 = glyph.t0;
		float s1 = glyph.s1;
		float t1 = glyph.t1;
		GLuint indices[6] = { 0, 1, 2, 0, 2, 3 };
		vertex_t vertices[4] = { { x0, y0, 0, s0, t0, r, g, b, a },
		{ x0, y1, 0, s0, t1, r, g, b, a },
		{ x1, y1, 0, s1, t1, r, g, b, a },
		{ x1, y0, 0, s1, t0, r, g, b, a } };
		buffer->push_back(vertices, 4, indices, 6);
		pen->x += glyph.advance_x;
	}
}

SL_Graphics::Text::Text(){

	_Font = SL_Font::Create_Font("Times New Roman", DEFAULT_FONT_BUILD_SIZE);
	buffer = Vertex_Buffer_Factory::Create("vertex:3f,tex_coord:2f,color:4f");

	_Shader.AddShader_FromFile(GL_VERTEX_SHADER, "../../shaders/distance-field-2.vert");
	_Shader.AddShader_FromFile(GL_FRAGMENT_SHADER, "../../shaders/distance-field-2.frag");
	_Shader.Finalize();	
	Set_Font_Size(14);
	Set_Text("A Quick Brown Fox Jumps Over The Lazy Dog 0123456789");
	_Pos = glm::zero<glm::vec2>();
}

SL_Graphics::Text::~Text(){
	
}
void SL_Graphics::Text::Set_Text(std::string text, float red, float green, float blue, float alpha){
	_Text = text;
	glm::vec2 pen = glm::zero<glm::vec2>();
	glm::vec4 black(red, green, blue, alpha);
	add_text(buffer.get(), _Font, _Text.c_str(), &black, &pen);
}


void SL_Graphics::Text::Clear(){
	_Text.clear();
}

void SL_Graphics::Text::Draw(){
	if (_Text.empty()) return;
	static glm::mat4 model, view, projection;

	view = glm::mat4();
	_Shader.Enable();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _Font.Get_Font()->Texture_ID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int v[4];
	glGetIntegerv(GL_VIEWPORT, v);
	auto width = v[2];
	auto height = v[3];
	auto scaling = (_Font_Size / DEFAULT_FONT_BUILD_SIZE)*1.35f;
	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	model = glm::translate(-_Pos.x, height - _Font_Size - _Pos.y, 0.0f) * glm::scale(scaling, scaling, 1.0f);

	glUniform1i(_Shader.GetUniformLocation("texture"), 0);

	glUniformMatrix4fv(_Shader.GetUniformLocation("model"), 
		1, 0, glm::value_ptr(model));
	glUniformMatrix4fv(_Shader.GetUniformLocation("view"),
		1, 0, glm::value_ptr(view));
	glUniformMatrix4fv(_Shader.GetUniformLocation("projection"),
		1, 0, glm::value_ptr(projection));
	buffer->draw(-1, GL_TRIANGLES);
}
