#ifndef SL_VERTEX_BUFFER_FACTORY_123_H
#define SL_VERTEX_BUFFER_FACTORY_123_H
#include "Vertex_Buffer.h"
#include <string>

namespace SL_Graphics{
	namespace Vertex_Buffer_Factory{
		
		Model::Vertex_Buffer Create(std::string format);
	};
};
#endif