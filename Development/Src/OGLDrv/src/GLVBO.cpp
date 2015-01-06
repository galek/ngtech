#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	GLVBO *GLVBO::createVBO(void *_data, int numElements, int elemSize, DataType dataType, TypeDraw drawType) {
		GLVBO *vbo = new GLVBO();
		vbo->mBUFType = BUF_VERTEX;

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;
		vbo->data = _data;

		if (dataType == FLOAT) {
			vbo->dataType = GL_FLOAT;
		}
		else if (dataType == DOUBLE) {
			vbo->dataType = GL_DOUBLE;
		}

		if (drawType == STREAM)
			vbo->drawType = GL_STREAM_DRAW;
		else if (drawType == STATIC)
			vbo->drawType = GL_STATIC_DRAW;
		else if (drawType == DYNAMIC)
			vbo->drawType = GL_DYNAMIC_DRAW;

		vbo->type = GL_ARRAY_BUFFER;
		// создадим Vertex Buffer Object (VBO)
		glGenBuffers(1, &vbo->glID);

		// устанавливаем созданный VBO как текущий
		glBindBuffer(vbo->type, vbo->glID);

		// заполним VBO данными
		glBufferData(vbo->type, vbo->elementSize * vbo->numElements, vbo->data, vbo->drawType);

		vbo->unset();

		return vbo;
	}

	/**
	*/
	GLVBO *GLVBO::createIBO(void *_data, int numElements, int elemSize, DataType dataType) {
		GLVBO *vbo = new GLVBO();
		vbo->mBUFType = BUF_INDEX;

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;
		vbo->data = _data;

		if (dataType == UNSIGNED_INT)
			vbo->dataType = GL_UNSIGNED_INT;
		else if (dataType == UNSIGNED_SHORT)
			vbo->dataType = GL_UNSIGNED_SHORT;


		vbo->drawType = GL_STREAM_DRAW;
		vbo->type = GL_ELEMENT_ARRAY_BUFFER;

		glGenBuffers(1, &vbo->glID);
		glBindBuffer(vbo->type, vbo->glID);

		if (numElements <= 65536){
			vbo->dataType = GL_UNSIGNED_SHORT;
			vbo->elementSize = sizeof(unsigned short);
			glBufferData(vbo->type, sizeof(unsigned short) * vbo->numElements, vbo->data, vbo->drawType);
			MeshConvertIndicesTo<unsigned short>((int *)vbo->data, (int*)vbo->data, numElements);
		}
		else {
			vbo->dataType = GL_UNSIGNED_INT;
			vbo->drawType = GL_STATIC_DRAW;
			vbo->elementSize = sizeof(int);
			glBufferData(vbo->type, sizeof(int) * numElements, vbo->data, vbo->drawType);
		}

		vbo->unset();

		return vbo;
	}

	/**
	*/
	GLVBO::~GLVBO() {
		glDeleteBuffers(1, &glID);
	}

	/**
	*/
	void GLVBO::set() {
		glBindBuffer(type, glID);
	}

	/**
	*/
	void GLVBO::FillBuffer() {
		// заполним VBO данными
		glBufferData(type, elementSize * numElements, data, drawType);
	}

	/**
	*/
	void GLVBO::unset() {
		glBindBuffer(type, 0);
	}

	/**
	*/
	void GLVBO::setVertexSource(int numComp, int stride, int offset) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(numComp, dataType, stride, (void*)offset);
	}

	/**
	*/
	void GLVBO::setNormalSource(int stride, int offset) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(dataType, stride, (void*)offset);
	}

	/**
	*/
	void GLVBO::setTexCoordSource(int tex_unit, int numComp, int stride, int offset) {
		glClientActiveTexture(GL_TEXTURE0 + tex_unit);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(numComp, dataType, stride, (void*)offset);
	}

	/**
	*/
	void GLVBO::setIndexSource(int stride, int offset) {
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(dataType, stride, (void*)offset);
	}

	/**
	*/
	void GLVBO::unsetVertexSource() {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	/**
	*/
	void GLVBO::unsetNormalSource() {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	/**
	*/
	void GLVBO::unsetTexCoordSource(int tex_unit) {
		glClientActiveTexture(GL_TEXTURE0 + tex_unit);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	/**
	*/
	void GLVBO::unsetIndexSource() {
		glDisableClientState(GL_INDEX_ARRAY);
	}

	/**
	*/
	void * GLVBO::map(int offset, void** data) {
		set();

		if (this->mBUFType == BUF_VERTEX)
		{
			//if (flags == 0)
			//	flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
			GLint vertex_flags = GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
			vertexdata_locked.ptr = glMapBufferRange(GL_ARRAY_BUFFER, offset, elementSize, vertex_flags);
			vertexdata_locked.flags = vertex_flags;
			if (!vertexdata_locked.ptr)
				return NULL;

			if (data)
				(*data) = vertexdata_locked.ptr;

			return vertexdata_locked.ptr;
		}
		else if (this->mBUFType == BUF_INDEX)
		{
			GLint index_flags = GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT;
			indexdata_locked.ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, elementSize, index_flags);
			indexdata_locked.flags = index_flags;

			if (!indexdata_locked.ptr)
				return NULL;

			if (data)
				(*data) = indexdata_locked.ptr;

			return indexdata_locked.ptr;
		}
		else
		{
			Error("NOT DETERMINATED", true);
		}
		return nullptr;
	}

	// !TODO
	// !BUG:ругается Buffer is unbound or is already unmapped.
	/**
	*/
	void GLVBO::unMap() {
		if (this->mBUFType == BUF_VERTEX)
		{
			if (vertexdata_locked.ptr && glID != 0)
			{
				glUnmapBuffer(type);
				vertexdata_locked.ptr = 0;
			}
		}
		else if (this->mBUFType == BUF_INDEX)
		{
			if (indexdata_locked.ptr && glID != 0)
			{
				glUnmapBuffer(type);
				indexdata_locked.ptr = 0;
			}
		}
		unset();
	}

}