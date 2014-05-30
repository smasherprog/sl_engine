#include "stdafx.h"
#include "Vertex_Buffer.h"

void SL_Graphics::Model::Vertex_Buffer::clear(){
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
	for (auto& a : attributes) a = Vector_Attribute();

}

void SL_Graphics::Model::Vertex_Buffer::insert_vertices(const size_t index, const void* verts, const size_t count){
	state = DIRTY;
	vertices.insert(index, verts, count);
}
void SL_Graphics::Model::Vertex_Buffer::insert_indices(const size_t index, const void* ind, const size_t count){
	state = DIRTY;
	indices.insert(index, ind, count);
}
size_t SL_Graphics::Model::Vertex_Buffer::insert(const size_t index, const void * v, const size_t vcount, const GLuint * ind, const size_t icount){
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

void SL_Graphics::Model::Vertex_Buffer::upload(){
	size_t vsize, isize;
	if (state == FROZEN) return;
	bool firstvertbuffer = false;
	if (vertices_id == 0) {
		glGenBuffers(1, &vertices_id);
		firstvertbuffer = true;
	}
	if (indices_id == 0) glGenBuffers(1, &indices_id);

	vsize = vertices.size()*vertices.stride();
	isize = indices.size()*indices.stride();

	// Always upload vertices first such that indices do not point to non
	// existing data (if we get interrupted in between for example).

	// Upload vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	if (vsize != GPU_vsize)
	{
		glBufferData(GL_ARRAY_BUFFER, vsize, vertices.front(), GL_DYNAMIC_DRAW);
		GPU_vsize = vsize;
	}
	else glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, vertices.front());
	if (firstvertbuffer){
		for (auto i = 0; i < MAX_VERTEX_ATTRIBUTE; ++i) attributes[i].enable();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Upload indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	if (isize != GPU_isize)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.front(), GL_DYNAMIC_DRAW);
		GPU_isize = isize;
	}
	else glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, isize, indices.front());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void SL_Graphics::Model::Vertex_Buffer::draw_begin(GLenum m){
	size_t i;
	if (state != CLEAN)
	{
		upload();
		state = CLEAN;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	if (!indices.empty()) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	mode = mode;
}
void SL_Graphics::Model::Vertex_Buffer::draw_end(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void SL_Graphics::Model::Vertex_Buffer::draw(size_t index, GLenum m){
	ivec4 item;
	if (index != -1){
		assert(index < items.size());
		item = *(ivec4*)items.get(index);
	}
	else {
		item.icount = indices.size();
		item.istart = 0;
		item.vstart = 0;
		item.vcount = vertices.size();
	}

	draw_begin(mode);
	if (item.icount > 0) glDrawElements(mode, item.icount, GL_UNSIGNED_INT, (void *)(item.istart*sizeof(GLuint)));
	else glDrawArrays(mode, item.vstart, item.vcount);
	draw_end();
}
