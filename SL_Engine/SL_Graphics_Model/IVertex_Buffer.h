#ifndef SL_IVERTEX_BUFFER_H
#define SL_IVERTEX_BUFFER_H
#include <string>
#include <vector>
#include "Vector_T.h"
#include "IVertex_Attribute.h"
#include "..\glew\include\GL\glew.h"
#include "..\glfw\include\GLFW\glfw3.h"
#include <memory>

namespace SL_Graphics{
	namespace Model{

		typedef union
		{
			int data[4];    /**< All compoments at once     */
			struct {
				int x;      /**< Alias for first component  */
				int y;      /**< Alias for second component */
				int z;      /**< Alias for third component  */
				int w;      /**< Alias for fourht component */
			};
			struct {
				int x_;     /**< Alias for first component  */
				int y_;     /**< Alias for second component */
				int width;  /**< Alias for third component  */
				int height; /**< Alias for fourth component */
			};
			struct {
				int r;      /**< Alias for first component  */
				int g;      /**< Alias for second component */
				int b;      /**< Alias for third component  */
				int a;      /**< Alias for fourth component */
			};
			struct {
				int red;    /**< Alias for first component  */
				int green;  /**< Alias for second component */
				int blue;   /**< Alias for third component  */
				int alpha;  /**< Alias for fourth component */
			};
			struct {
				int vstart; /**< Alias for first component  */
				int vcount; /**< Alias for second component */
				int istart; /**< Alias for third component  */
				int icount; /**< Alias for fourth component */
			};
		} ivec4;

		class IVertex_Buffer{
		public:

			IVertex_Buffer() { IVertex_Buffer::clear(); }
			virtual ~IVertex_Buffer() { clear(); }
			
			void push_back_vertices(const void* verts, const size_t count){ state = DIRTY; vertices.push_back(verts, count); }
			void push_back_indices(GLuint* ind, const size_t count){ state = DIRTY; auto b = vertices.size(); for (size_t i = 0; i < count; i++) ind[i] += b; indices.push_back(ind, count); }
			void insert_vertices(const size_t index, const void* verts, const size_t count);
			void insert_indices(const size_t index, const void* ind, const size_t count);
			size_t insert(const size_t index, const void * vertices, const size_t vcount, const GLuint * indices, const size_t icount);
			size_t push_back(const void * vertices, const size_t vcount, const GLuint * indices, const size_t icount){ return insert(items.size(), vertices, vcount, indices, icount); }
			
			virtual void clear();
			virtual void upload() = 0;
			virtual void draw_begin(GLenum m) = 0;
			virtual void draw_end() = 0;
			virtual void draw(size_t index, GLenum m) = 0;

			size_t batch_count() const { return items.size(); }

			enum Status { CLEAN, DIRTY, FROZEN };
			/** Format of the vertex buffer. */
			std::string format = "";

			/** Vector of vertices. */
			Vector_T vertices;

			/** GL identity of the vertices buffer. */
			GLuint vertices_id = 0;

			/** Vector of indices. */
			Vector_T indices;

			/** GL identity of the indices buffer. */
			GLuint indices_id = 0;

			/** Current size of the vertices buffer in GPU */
			size_t GPU_vsize = 0;

			/** Current size of the indices buffer in GPU*/
			size_t GPU_isize = 0;

			/** GL primitives to render. */
			GLenum mode = 0;

			/** Vector of items. */
			Vector_T items;

			/** Whether the vertex buffer needs to be uploaded to GPU memory. */
			Status state = CLEAN;

			/** Array of attributes. */
			std::shared_ptr<IVertex_Attribute> attributes[MAX_VERTEX_ATTRIBUTE];

		};
	};
};

#endif