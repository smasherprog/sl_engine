#ifndef SL_VERTEX_BUFFER_FACTORY_123_H
#define SL_VERTEX_BUFFER_FACTORY_123_H
#include "Vertex_Buffer.h"
#include <string>
#include <memory>

namespace SL_Graphics{
	namespace Vertex_Buffer_Factory{
		std::shared_ptr<Model::IVertex_Buffer> Create(std::string format);
	};
};
#endif