#include <assert.h>
#include "I_FFP.h"

namespace NGTech
{
	/*
	 */
	enum {
		FFP_NONE = 0,
		FFP_LINES,
		FFP_TRIANGLES,
	};

	/*
	 */
	struct FfpData {

		I_FFP *ffp;					// own ffp

		int primitive;				// primitive

		int num_vertex;				// vertex buffer
		int vertex_capacity;
		I_FFP::Vertex *vertex;

		int num_indices;			// indices buffer
		int indices_capacity;
		unsigned short *indices;
	};

	static FfpData *ffp = NULL;

	/*
	 */
	I_FFP::I_FFP() {

		assert(ffp == NULL && "Ffp::Ffp(): is already created");

		ffp = new FfpData();
		memset(ffp, 0, sizeof(FfpData));

		ffp->ffp = this;
	}

	I_FFP::~I_FFP() {

		delete[] ffp->vertex;
		delete[] ffp->indices;

		delete ffp;
		ffp = NULL;
	}

	/*
	 */
	I_FFP *I_FFP::get() {
		if (ffp == NULL) return NULL;
		return ffp->ffp;
	}

	/******************************************************************************\
	*
	* Primitives
	*
	\******************************************************************************/

	/*
	 */
	int I_FFP::isLines() const {
		return (ffp->primitive == FFP_LINES);
	}

	void I_FFP::beginLines() {

		assert(ffp->primitive == FFP_NONE && "Ffp::beginLines(): bad primitive");
		ffp->primitive = FFP_LINES;

		ffp->num_vertex = 0;
		ffp->num_indices = 0;
	}

	void I_FFP::endLines() {

		assert(ffp->primitive == FFP_LINES && "Ffp::endLines(): bad primitive");
		ffp->primitive = FFP_NONE;

		if (ffp->num_vertex == 0) return;
		if (ffp->num_indices == 0) return;

		render_lines(ffp->vertex, ffp->num_vertex, ffp->indices, ffp->num_indices);
	}

	/*
	 */
	int I_FFP::isTriangles() const {
		return (ffp->primitive == FFP_TRIANGLES);
	}

	void I_FFP::beginTriangles() {

		assert(ffp->primitive == FFP_NONE && "Ffp::beginTriangles(): bad primitive");
		ffp->primitive = FFP_TRIANGLES;

		ffp->num_vertex = 0;
		ffp->num_indices = 0;
	}

	void I_FFP::endTriangles() {

		assert(ffp->primitive == FFP_TRIANGLES && "Ffp::endTriangles(): bad primitive");
		ffp->primitive = FFP_NONE;

		if (ffp->num_vertex == 0) return;
		if (ffp->num_indices == 0) return;

		render_triangles(ffp->vertex, ffp->num_vertex, ffp->indices, ffp->num_indices);
	}

	/*
	 */
	void I_FFP::renderScreen() {

		render_screen();
	}

	/******************************************************************************\
	*
	* Vertices
	*
	\******************************************************************************/

	/*
	 */
	static void reserve_vertex(int num_vertex) {
		if (ffp->vertex_capacity < ffp->num_vertex + num_vertex) {
			ffp->vertex_capacity = (ffp->vertex_capacity + num_vertex) * 2;
			if (ffp->vertex_capacity < 1024) ffp->vertex_capacity = 1024;
			I_FFP::Vertex *vertex = new I_FFP::Vertex[ffp->vertex_capacity];
			memcpy(vertex, ffp->vertex, sizeof(I_FFP::Vertex) * ffp->num_vertex);
			delete[] ffp->vertex;
			ffp->vertex = vertex;
		}
	}

	static ENGINE_INLINE I_FFP::Vertex &add_vertex() {
		reserve_vertex(1);
		return ffp->vertex[ffp->num_vertex++];
	}

	static ENGINE_INLINE I_FFP::Vertex &get_vertex() {
		assert(ffp->num_vertex > 0 && "get_vertex(): vertices is empty");
		return ffp->vertex[ffp->num_vertex - 1];
	}

	/*
	 */
	int I_FFP::getNumVertex() const {
		return ffp->num_vertex;
	}

	void I_FFP::reserveVertex(int num_vertex) const {
		reserve_vertex(num_vertex);
	}

	void I_FFP::addVertex(const Vertex &vertex) const {
		reserve_vertex(1);
		Vertex &v = ffp->vertex[ffp->num_vertex++];
		v.xyz[0] = vertex.xyz[0];
		v.xyz[1] = vertex.xyz[1];
		v.xyz[2] = vertex.xyz[2];
		v.texcoord[0] = vertex.texcoord[0];
		v.texcoord[1] = vertex.texcoord[1];
		v.texcoord[2] = vertex.texcoord[2];
		v.texcoord[3] = vertex.texcoord[3];
		v.color_vec = vertex.color_vec;
	}

	void I_FFP::addVertexFast(const Vertex &vertex) const {
		Vertex &v = ffp->vertex[ffp->num_vertex++];
		v.xyz[0] = vertex.xyz[0];
		v.xyz[1] = vertex.xyz[1];
		v.xyz[2] = vertex.xyz[2];
		v.texcoord[0] = vertex.texcoord[0];
		v.texcoord[1] = vertex.texcoord[1];
		v.texcoord[2] = vertex.texcoord[2];
		v.texcoord[3] = vertex.texcoord[3];
		v.color_vec = vertex.color_vec;
	}

	void I_FFP::addVertex(const Vertex *vertex, int num_vertex) const {
		reserve_vertex(num_vertex);
		memcpy(ffp->vertex + ffp->num_vertex, vertex, sizeof(I_FFP::Vertex) * num_vertex);
		ffp->num_vertex += num_vertex;
	}

	void I_FFP::addVertex(const float *xyz, int size) const {
		assert(size > 0 && size < 4 && "Ffp::addVertex(): bad coordinate size");
		Vertex &v = add_vertex();
		for (int i = 0; i < 3; i++) {
			if (i < size) v.xyz[i] = xyz[i];
			else v.xyz[i] = 0.0f;
		}
		v.texcoord[0] = 0.0f;
		v.texcoord[1] = 0.0f;
		v.texcoord[2] = 0.0f;
		v.texcoord[3] = 0.0f;
		v.color_vec = 0xffffffff;
	}

	void I_FFP::addVertex(float x, float y, float z) const {
		Vertex &v = add_vertex();
		v.xyz[0] = x;
		v.xyz[1] = y;
		v.xyz[2] = z;
		v.texcoord[0] = 0.0f;
		v.texcoord[1] = 0.0f;
		v.texcoord[2] = 0.0f;
		v.texcoord[3] = 0.0f;
		v.color_vec = 0xffffffff;
	}

	/*
	 */
	void I_FFP::setTexCoord(const float *texcoord, int size) const {
		assert(size > 0 && size < 5 && "Ffp::setTexCoord(): bad texture coordinate size");
		Vertex &v = get_vertex();
		for (int i = 0; i < 4; i++) {
			if (i < size) v.texcoord[i] = texcoord[i];
			else v.texcoord[i] = 0.0f;
		}
	}

	void I_FFP::setTexCoord(float x, float y, float z, float w) const {
		Vertex &v = get_vertex();
		v.texcoord[0] = x;
		v.texcoord[1] = y;
		v.texcoord[2] = z;
		v.texcoord[3] = w;
	}

	/*
	 */
	void I_FFP::setColor(const float *color, int size) const {
		assert(size > 0 && size < 5 && "Ffp::setColor(): bad coordinate size");
		Vertex &v = get_vertex();
		for (int i = 0; i < 4; i++) {
			if (i < size) v.color[i] = (unsigned char)(color[i] * 255.0f);
			else v.color[i] = 255;
		}
	}

	void I_FFP::setColor(float r, float g, float b, float a) const {
		Vertex &v = get_vertex();
		v.color[0] = (unsigned char)(r * 255.0f);
		v.color[1] = (unsigned char)(g * 255.0f);
		v.color[2] = (unsigned char)(b * 255.0f);
		v.color[3] = (unsigned char)(a * 255.0f);
	}

	void I_FFP::setColor(const unsigned char *color, int size) const {
		assert(size > 0 && size < 5 && "Ffp::setColor(): bad coordinate size");
		Vertex &v = get_vertex();
		for (int i = 0; i < 4; i++) {
			if (i < size) v.color[i] = color[i];
			else v.color[i] = 255;
		}
	}

	void I_FFP::setColor(int r, int g, int b, int a) const {
		Vertex &v = get_vertex();
		v.color[0] = (unsigned char)r;
		v.color[1] = (unsigned char)g;
		v.color[2] = (unsigned char)b;
		v.color[3] = (unsigned char)a;
	}

	void I_FFP::setColor(unsigned int color) const {
		Vertex &v = get_vertex();
		v.color[0] = (unsigned char)((color >> 16) & 0xff);
		v.color[1] = (unsigned char)((color >> 8) & 0xff);
		v.color[2] = (unsigned char)((color >> 0) & 0xff);
		v.color[3] = (unsigned char)((color >> 24) & 0xff);
	}

	/******************************************************************************\
	*
	* Indices
	*
	\******************************************************************************/

	/*
	 */
	static void reserve_indices(int num_indices) {
		if (ffp->indices_capacity < ffp->num_indices + num_indices) {
			ffp->indices_capacity = (ffp->indices_capacity + num_indices) * 2;
			if (ffp->indices_capacity < 1024) ffp->indices_capacity = 1024;
			unsigned short *indices = new unsigned short[ffp->indices_capacity];
			memcpy(indices, ffp->indices, sizeof(unsigned short) * ffp->num_indices);
			delete[] ffp->indices;
			ffp->indices = indices;
		}
	}

	/*
	 */
	int I_FFP::getNumIndices() const {
		return ffp->num_indices;
	}

	void I_FFP::reserveIndices(int num_indices) const {
		reserve_indices(num_indices);
	}

	void I_FFP::addIndex(int index) const {
		reserve_indices(1);
		ffp->indices[ffp->num_indices++] = (unsigned short)index;
	}

	void I_FFP::addIndexFast(int index) const {
		ffp->indices[ffp->num_indices++] = (unsigned short)index;
	}

	void I_FFP::addIndices(int i0, int i1) const {
		reserve_indices(2);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		indices[0] = (unsigned short)i0;
		indices[1] = (unsigned short)i1;
		ffp->num_indices += 2;
	}

	void I_FFP::addIndices(int i0, int i1, int i2) const {
		reserve_indices(3);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		indices[0] = (unsigned short)i0;
		indices[1] = (unsigned short)i1;
		indices[2] = (unsigned short)i2;
		ffp->num_indices += 3;
	}

	void I_FFP::addIndices(int i0, int i1, int i2, int i3) const {
		reserve_indices(4);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		indices[0] = (unsigned short)i0;
		indices[1] = (unsigned short)i1;
		indices[2] = (unsigned short)i2;
		indices[3] = (unsigned short)i3;
		ffp->num_indices += 4;
	}

	void I_FFP::addIndices(const unsigned short *indices, int num_indices) const {
		reserve_indices(num_indices);
		memcpy(ffp->indices + ffp->num_indices, indices, sizeof(unsigned short) * num_indices);
		ffp->num_indices += num_indices;
	}

	void I_FFP::addIndices(const unsigned short *src, int num_indices, int vertex_offset) const {
		reserve_indices(num_indices);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		for (int i = 0; i < num_indices; i++) {
			indices[i] = (unsigned short)(src[i] + vertex_offset);
		}
		ffp->num_indices += num_indices;
	}

	/*
	 */
	void I_FFP::addLines(int num) const {
		reserve_indices(num * 2);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		for (int i = 0, j = ffp->num_vertex; i < num; i++, j += 2) {
			indices[0] = (unsigned short)(j + 0);
			indices[1] = (unsigned short)(j + 1);
			indices += 2;
		}
		ffp->num_indices += num * 2;
	}

	void I_FFP::addTriangles(int num) const {
		reserve_indices(num * 3);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		for (int i = 0, j = ffp->num_vertex; i < num; i++, j += 3) {
			indices[0] = (unsigned short)(j + 0);
			indices[1] = (unsigned short)(j + 1);
			indices[2] = (unsigned short)(j + 2);
			indices += 3;
		}
		ffp->num_indices += num * 3;
	}

	void I_FFP::addTriangleQuads(int num) const {
		reserve_indices(num * 6);
		unsigned short *indices = ffp->indices + ffp->num_indices;
		for (int i = 0, j = ffp->num_vertex; i < num; i++, j += 4) {
			indices[0] = (unsigned short)(j + 0);
			indices[1] = (unsigned short)(j + 1);
			indices[2] = (unsigned short)(j + 2);
			indices[3] = (unsigned short)(j + 2);
			indices[4] = (unsigned short)(j + 3);
			indices[5] = (unsigned short)(j + 0);
			indices += 6;
		}
		ffp->num_indices += num * 6;
	}
}