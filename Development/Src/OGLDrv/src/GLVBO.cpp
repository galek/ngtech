#include "RenderPrivate.h"
//***************************************************************************
#include <algorithm>
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
#include "GLSynch.h"
//***************************************************************************

namespace NGTech {

	GLsync vertex_sync[3];
	/**
	*/
	GLVBO::GLVBO()
	{
		data = nullptr;
		numElements = 0;
		frame = 0;
		glID = 0;
		_size = 0;
		vertex_offset = 0;
		vertexdata_locked.ptr = 0;
		indexdata_locked.ptr = 0;
		num_indices = 0;
		_Create();
	}

	/**
	*/
	void GLVBO::DeleteBuffers()
	{
		if (vertex_sync[0] && glIsSync(vertex_sync[0])) glDeleteSync(vertex_sync[0]);
		if (vertex_sync[1] && glIsSync(vertex_sync[1])) glDeleteSync(vertex_sync[1]);
		if (vertex_sync[2] && glIsSync(vertex_sync[2])) glDeleteSync(vertex_sync[2]);
		vertexdata_locked.ptr = 0;
		indexdata_locked.ptr = 0;
		vertex_sync[0] = NULL;
		vertex_sync[1] = NULL;
		vertex_sync[2] = NULL;

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

		if (numElements <= 65536) {
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
	void * GLVBO::map(int offset, void** _data)
	{
		if (this->mBUFType == BUF_VERTEX)
		{
			return _ResizeBuffer(vertexdata_locked, offset, _data);
		}
		else if (this->mBUFType == BUF_INDEX)
		{
			return _ResizeBuffer(indexdata_locked, offset, _data);
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
		// update vertices buffer
		vertex_offset += buffer_size;
		buffer_size = sizeof(data);//Nick?

		// per-frame buffers
		if (frame != GetWindow()->frame) {
			frame = GetWindow()->frame;
			numElements = buffer_size;
		}
		else {
			numElements += buffer_size;
		}

		if (num_indices < numElements * 3)
		{
			num_indices = std::max(num_indices * 2, numElements * 3);

			// synchronization
			GLSynch::waitSync(vertex_sync);

			// delete vbo
			DeleteBuffers();//Unigine style
			// create vbo
			_Create();//Unigine style
			Bind();//Unigine style

			GLint index_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT /*| GL_MAP_UNSYNCHRONIZED_BIT*/;
			glBufferStorage(this->type, this->elementSize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT);
			_data.ptr = glMapBufferRange(this->type, offset, this->elementSize, index_flags);
			_data.flags = index_flags;
			vertex_offset = 0;

			if (!_data.ptr)
			{
				Error("GLExt::render(): can't map buffer\n", true);
				return NULL;
			}

			if (data)
				(*data) = _data.ptr;

			// update vertices vao
			//update_vertex_array();
		}

		// synchronization
		GLSynch::waitSync(vertex_sync, vertex_offset, buffer_size, num_indices);

		// copy vertices
		//Math::memcpy(_data.ptr + vertex_offset, vertex.get(), vertex_flush);

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