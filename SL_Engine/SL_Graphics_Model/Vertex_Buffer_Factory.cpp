#include "stdafx.h"
#include "Vertex_Buffer_Factory.h"
#include "Vertex_Attribute.h"
#include "..\SL_Utilities\String.h"

SL_Graphics::Model::Vertex_Buffer SL_Graphics::Vertex_Buffer_Factory::Create(std::string format){

	Model::Vertex_Buffer self;
	self.format = format;
	auto index = 0;
	auto stride = 0;
	auto splits = split(format, ',');
	if (splits.size() > MAX_VERTEX_ATTRIBUTE) splits.resize(index < MAX_VERTEX_ATTRIBUTE);
	for (auto& token: splits){
		GLuint attribute_size = 0;
		auto& attribute = self.attributes[index];
		if (!Model::Vector_Attribute::Parse(token, attribute)) continue;

		switch (attribute.type)
		{
		case GL_BOOL:           attribute_size = sizeof(GLboolean); break;
		case GL_BYTE:           attribute_size = sizeof(GLbyte); break;
		case GL_UNSIGNED_BYTE:  attribute_size = sizeof(GLubyte); break;
		case GL_SHORT:          attribute_size = sizeof(GLshort); break;
		case GL_UNSIGNED_SHORT: attribute_size = sizeof(GLushort); break;
		case GL_INT:            attribute_size = sizeof(GLint); break;
		case GL_UNSIGNED_INT:   attribute_size = sizeof(GLuint); break;
		case GL_FLOAT:          attribute_size = sizeof(GLfloat); break;
		default:                attribute_size = 0;
		}
		attribute.offset = stride;
		stride += attribute.size*attribute_size;
		index++;
	}


	for (auto i = 0; i < index; ++i) self.attributes[i].stride = stride;


	self.vertices = Model::Vector_T(stride);
	self.vertices_id = 0;
	self.GPU_vsize = 0;

	self.indices = Model::Vector_T(sizeof(GLuint));
	self.indices_id = 0;
	self.GPU_isize = 0;

	self.items = Model::Vector_T(sizeof(Model::ivec4));
	self.state = SL_Graphics::Model::Vertex_Buffer::Status::DIRTY;
	self.mode = GL_TRIANGLES;
	return self;
}