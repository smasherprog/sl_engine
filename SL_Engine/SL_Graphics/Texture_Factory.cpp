#include "stdafx.h"
#include "Texture_Factory.h"
#include "..\SL_Graphics_Model\ITexture.h"
#include "..\SL_Graphics_Model\Graphics_Debug.h"

std::map<std::string, SL_Graphics::ITexture> SL_Graphics::Texture_Factory::_INTERNAL::_Texture_Cache;

std::shared_ptr<SL_Graphics::ITexture> SL_Graphics::Texture_Factory::Create(const std::string& str){

	return nullptr;
}


void SL_Graphics::Texture_Factory::_INTERNAL::UnLoad(ITexture* tex){


}
std::shared_ptr<SL_Graphics::ITexture> Create(unsigned int size_x, unsigned int size_y, int filter = GL_LINEAR, int repeat = GL_CLAMP_TO_EDGE, int type1 = GL_RGBA, int type2 = GL_RGBA, unsigned char* data = 0, int type3 = GL_UNSIGNED_BYTE){
	int id = 0;

	glGenTextures(1, (GLuint *)&id); CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, id); CHECK_GL_ERROR();

	// set basic parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat); CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat); CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (filter == GL_LINEAR_MIPMAP_LINEAR) ? GL_LINEAR : filter); CHECK_GL_ERROR();

	// buffer data
	if (filter != GL_LINEAR_MIPMAP_LINEAR)
		glTexImage2D(GL_TEXTURE_2D, 0, type1, size_x, size_y, 0, type2, type3, data);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, type1, size_x, size_y, type2, type3, data);

	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, 0);
	std::shared_ptr<SL_Graphics::ITexture> ptr(new SL_Graphics::ITexture(SL_Graphics::Texture_Factory::_INTERNAL::UnLoad, id));
	return ptr;
}