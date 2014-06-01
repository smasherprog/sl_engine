#include "stdafx.h"
#include "IVertex_Buffer.h"

void SL_Graphics::Model::IVertex_Buffer::clear(){
	format = "";
	vertices.clear();
	vertices_id = 0;
	indices.clear();
	items.clear();
	indices_id = 0;
	GPU_vsize = 0;
	GPU_isize = 0;
	mode = 0;
	state = CLEAN;
}

void SL_Graphics::Model::IVertex_Buffer::insert_vertices(const size_t index, const void* verts, const size_t count){
	state = DIRTY;
	vertices.insert(index, verts, count);
}
void SL_Graphics::Model::IVertex_Buffer::insert_indices(const size_t index, const void* ind, const size_t count){
	state = DIRTY;
	indices.insert(index, ind, count);
}
size_t SL_Graphics::Model::IVertex_Buffer::insert(const size_t index, const void * v, const size_t vcount, const GLuint * ind, const size_t icount){
	size_t vstart, istart, i;
	ivec4 item;

	state = FROZEN;

	// Push back vertic
	vstart = vertices.size();
	vertices.push_back(v, vcount);

	// Push back indices
	istart = indices.size();
	indices.push_back(ind, icount);

	// Update indices within the vertex buffer
	for (i = 0; i < icount; ++i)
	{
		*(GLuint *)(indices.get(istart + i)) += vstart;
	}

	// Insert item
	item.vstart = vstart;
	item.vcount = vcount;
	item.istart = istart;
	item.icount = icount;
	items.insert(index, &item, 1);

	state = DIRTY;
	return index;
}
