#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	GLVBO *GLVBO::createVBO(void *data, int numElements, int elemSize, DataType dataType, TypeDraw drawType) {
		GLVBO *vbo = new GLVBO();

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;

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

		vbo->type = GL_ARRAY_BUFFER;

		glGenBuffers(1, &vbo->glID);
		glBindBuffer(vbo->type, vbo->glID);
		glBufferData(vbo->type, vbo->elementSize * vbo->numElements, data, vbo->drawType);


		return vbo;
	}

	/**
	*/
	GLVBO *GLVBO::createIBO(void *data, int numElements, int elemSize, DataType dataType) {
		GLVBO *vbo = new GLVBO();

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;

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
			glBufferData(vbo->type, sizeof(unsigned short) * vbo->numElements, data, vbo->drawType);
			MeshConvertIndicesTo<unsigned short>((int *)data, (int*)data, numElements);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numElements, data, GL_STATIC_DRAW);
		}
		else {
			vbo->dataType = GL_UNSIGNED_INT;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numElements, data, GL_STATIC_DRAW);
		}
		return vbo;
	}

	/**
	*/
	GLVBO::~GLVBO() {
		glDeleteBuffersARB(1, &glID);
	}

	/**
	*/
	void GLVBO::set() {
		glBindBuffer(type, glID);
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
		glClientActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
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
		glClientActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	/**
	*/
	void GLVBO::unsetIndexSource() {
		glDisableClientState(GL_INDEX_ARRAY);
	}


}