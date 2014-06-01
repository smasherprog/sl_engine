#ifndef SL_VERTEX_BUFFER_H
#define SL_VERTEX_BUFFER_H
#include <vector>
#include "..\SL_Graphics_Model\IVertex_Buffer.h"
#include "Vertex_Attribute.h"

namespace SL_Graphics{
	namespace Model{

		class Vertex_Buffer : public IVertex_Buffer{
		public:
			Vertex_Buffer(){ 
				for (auto& a : attributes) a = std::make_shared<Vertex_Attribute>(); 
			}
			virtual ~Vertex_Buffer(){}

			virtual void clear(){
				IVertex_Buffer::clear();
				for (auto& a : attributes) a = std::make_shared<Vertex_Attribute>();
			}
			virtual void upload();
			virtual void draw_begin(GLenum m);
			virtual void draw_end();
			virtual void draw(size_t index, GLenum m);

		};
	};
};

#endif