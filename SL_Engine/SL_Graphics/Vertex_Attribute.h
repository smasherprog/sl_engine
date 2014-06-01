#ifndef SL_VERTEX_ATTRIBUTE_H_123_H
#define SL_VERTEX_ATTRIBUTE_H_123_H
#include <string>
#include "..\SL_Graphics_Model\IVertex_Attribute.h"

namespace SL_Graphics{
	namespace Model{

		class Vertex_Attribute : public IVertex_Attribute{
		public:
			virtual void enable();
		};
	};
};

#endif