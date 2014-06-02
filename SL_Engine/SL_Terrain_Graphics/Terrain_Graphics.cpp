#include "stdafx.h"
#include "Terrain_Graphics.h"
#include "..\SL_Graphics\Resources.h"
#include "..\SL_Graphics_Model\Shapes.h"
#include "..\SL_Graphics\Shader.h"
#include "..\SL_Utilities\Imaging.h"
#include "..\SL_Utilities\Debug_Output.h"
#include "..\glm\glm.hpp"
#include"..\glm\ext.hpp"
#include "..\SL_Graphics_Model\Camera.h"

SL_Terrain::Terrain_Graphics::~Terrain_Graphics(){
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VB);
	glDeleteBuffers(1, &_IB);
}
SL_Terrain::Terrain_Graphics::Terrain_Graphics(){
	DEBUG_MSG("BEGIN: %", __FUNCTION__);
	auto map = SL_Image::Load_X<float>(SL_Resources::Texture_Location + std::string("GameAreaHeightMap.png"));
	unsigned int mapsize = (unsigned int)sqrt(map.size());
	Add_Block(map, 0, 0, mapsize, mapsize, mapsize);
	mapsize = 64;
	auto indicies = SL_Graphics::Shapes::Generate_Grid_Indices<unsigned int>(mapsize, mapsize);

	auto gridvertex = SL_Graphics::Shapes::Generate_Grid_Vertices(mapsize, mapsize, map);
	num_indices = indicies.size();
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VB);
	glBindBuffer(GL_ARRAY_BUFFER, _VB);
	glBufferData(GL_ARRAY_BUFFER, gridvertex.size()*sizeof(glm::vec3), &gridvertex[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size()*sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);


	_Shader = std::make_unique<SL_Graphics::Shader>();
	std::string vert = SL_Resources::Shader_Location;
	vert += "Terrain.vert";
	std::string frag = SL_Resources::Shader_Location;
	frag += "Terrain.frag";
	_Shader->AddShader_FromFile(GL_VERTEX_SHADER, vert.c_str());
	_Shader->AddShader_FromFile(GL_FRAGMENT_SHADER, frag.c_str());
	_Shader->Finalize();
	matrix = _Shader->GetUniformLocation("perspectiveMatrix");
	DEBUG_MSG("END: %", __FUNCTION__);
}

void SL_Terrain::Terrain_Graphics::Draw(double dt, SL_Graphics::Camera* c){

	glm::mat4x4 pos(glm::translate(0.0f, 0.0f, -100.0f));

	auto comb = c->Get_Projection() * c->Get_View() * pos;
	glDisable(GL_CULL_FACE);
	_Shader->Enable();
	glBindVertexArray(_VAO);
	glUniformMatrix4fv(matrix, 1, GL_FALSE, &comb[0][0]);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}