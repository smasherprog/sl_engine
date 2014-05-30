#ifndef SL_VECTOR_T_123_H
#define SL_VECTOR_T_123_H
#include <vector>
#include "assert.h"

//facade for an std::vector that supports different element strides. I could use a template, but this is more general purpose
namespace SL_Graphics{
	namespace Model{
		class Vector_T{
			std::vector<char> Buffer;
			size_t Stride = 0;
		public:
			Vector_T() : Vector_T(0){}
			Vector_T(size_t element_stride) : Stride(element_stride) { }

			char* get(const size_t index){ assert(!empty()); return front() + (stride()*index); }
			char* back(){ assert(!empty()); return front() + (stride()*size()) - stride(); }
			char* front(){ return &Buffer[0]; }

			void reserve(size_t num_of_elements){ Buffer.reserve(num_of_elements*stride()); }
			bool empty() const { return Buffer.empty(); }
			size_t capacity() const { return Buffer.capacity() / stride(); }
			size_t stride() const { return Stride; }
			size_t size() const { return Stride == 0 ? 0 : Buffer.size() / Stride; }
			void clear(){ Buffer.clear(); }

			void resize(size_t newsize){ Buffer.resize(newsize*stride()); }
			void push_back(const void* item){ push_back(item, 1); }
			void push_back(const void* item, const size_t num_elements){ auto beforesize = size(); resize(size() + num_elements); memcpy((front() + (stride() * beforesize)), item, stride()*num_elements); }
			void pop_back(){ Buffer.pop_back(); }

			void insert(const size_t index, const void* data, const size_t count){
				auto beforesize = size();
				resize(beforesize + count);
				if (index < beforesize){
					memmove(front() + (index + 1) * stride(),
						front() + (index + 0) * stride(),
						(beforesize - index)  * stride());
				}
				memcpy(front() + index * stride(),
					data, stride()*count);
			}
		};
	};
};

#endif