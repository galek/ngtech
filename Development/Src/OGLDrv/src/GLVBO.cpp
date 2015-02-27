#include "RenderPrivate.h"
//***************************************************************************
//#include <algorithm>
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	GLVBO::GLVBO()
	{
		data = nullptr;
		glID = 0;
		_size = 0;
		vertexdata_locked.ptr = 0;
		indexdata_locked.ptr = 0;
		num_indices = 0;
		_Create();
	}

	/**
	*/
	void GLVBO::DeleteBuffers()
	{
		glDeleteBuffers(1, &glID);
	}
	/**
	*/
	GLVBO::~GLVBO() {
		DeleteBuffers();
	}

	/**
	*/
	GLVBO *GLVBO::createVBO(void *_data, int numElements, int elemSize, DataType dataType, TypeDraw _drawType)
	{
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

		if (_drawType == STREAM)
			vbo->drawType = GL_STREAM_DRAW;
		else if (_drawType == STATIC)
			vbo->drawType = GL_STATIC_DRAW;
		else if (_drawType == DYNAMIC)
			vbo->drawType = GL_DYNAMIC_DRAW;

		vbo->type = GL_ARRAY_BUFFER;

		vbo->Allocate(vbo->data, vbo->elementSize * vbo->numElements, _drawType);

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

		if (numElements <= 65536){
			vbo->dataType = GL_UNSIGNED_SHORT;
			vbo->elementSize = sizeof(unsigned short);
			vbo->Allocate(vbo->data, sizeof(unsigned short) * vbo->numElements, STREAM);
			MeshConvertIndicesTo<unsigned short>((int *)vbo->data, (int*)vbo->data, numElements);
		}
		else {
			vbo->dataType = GL_UNSIGNED_INT;
			vbo->drawType = GL_STATIC_DRAW;
			vbo->elementSize = sizeof(int);
			vbo->Allocate(vbo->data, sizeof(int) * vbo->numElements, STATIC);
		}

		return vbo;
	}

	/**
	*/
	void GLVBO::_Create() {
		glGenBuffers(1, &glID);
	}

	/**
	*/
	void GLVBO::Bind() const
	{
		glBindBuffer(type, glID);
	}

	/**
	*/
	void GLVBO::UnBind() const
	{
		glBindBuffer(type, 0);
	}

	/**
	*/
	void GLVBO::BindIndex(unsigned int idx) const
	{
		glBindBufferBase(type, idx, glID);
	}

	/**
	*/
	void GLVBO::UnbindIndex(unsigned int idx) const
	{
		glBindBufferBase(type, idx, 0);
	}

	/**
	*/
	void GLVBO::Allocate(const void *data, size_t size, TypeDraw _drawType)
	{
		_size = size;
		GLSystem::RecordMemoryWrite(size);
		if (_drawType == STREAM)
			drawType = GL_STREAM_DRAW;
		else if (_drawType == STATIC)
			drawType = GL_STATIC_DRAW;
		else if (_drawType == DYNAMIC)
			drawType = GL_DYNAMIC_DRAW;
		glNamedBufferDataEXT(glID, size, data, drawType);
	}

	/**
	*/
	void GLVBO::FillBuffer(size_t offset) {
		// заполним VBO данными
		GLSystem::RecordMemoryWrite(elementSize * numElements);
		glNamedBufferSubDataEXT(glID, offset, elementSize * numElements, data);
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
	void * GLVBO::map(int offset, void** data)
	{
		if (this->mBUFType == BUF_VERTEX)
		{
			return _ResizeBuffer(vertexdata_locked, offset, data);
		}
		else if (this->mBUFType == BUF_INDEX)
		{
			return _ResizeBuffer(indexdata_locked, offset, data);
		}
		else
		{
			Error("NOT DETERMINATED", true);
		}
		return nullptr;
	}

	/**
	*/
	void GLVBO::unMap() {
		switch (this->mBUFType)
		{
		case BUF_VERTEX:
			DeleteBuffer(vertexdata_locked);
			break;
		case BUF_INDEX:
			DeleteBuffer(indexdata_locked);
			break;
		default:
			Error("GLVBO::unMap() undeterminated", true);
			break;
		}
	}

	/**
	*/
	void* GLVBO::_ResizeBuffer(locked_data _data, int offset, void** data)
	{
		if (num_indices < numElements * 3)
		{
			num_indices = max(num_indices * 2, numElements * 3);

			// delete vbo
			DeleteBuffers();
			// create vbo
			_Create();
			Bind();

			GLint index_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
			_data.ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, elementSize, index_flags);
			_data.flags = index_flags;

			if (!_data.ptr)
			{
				Error("GLExt::render(): can't map indices buffer\n", true);
				return NULL;
			}

			if (data)
				(*data) = _data.ptr;

		}
		return _data.ptr;
	}

	/**
	*/
	void GLVBO::DeleteBuffer(locked_data _data)
	{
		if (_data.ptr && glID != 0)
		{
			glUnmapBuffer(type);
			_data.ptr = 0;
		}
	}
}