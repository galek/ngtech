/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
//***************************************************************************

namespace NGEngine {

	//---------------------------------------------------------------------------
	//Desc:    creates vertex buffer object
	//Params:  data - pointer to data array, numElements - number of elements, elemSize - sizeof element, dataType - VBO data type
	//Returns: pointer to new GLVBO
	//---------------------------------------------------------------------------
	GLVBO *GLVBO::createVBO(void *data, int numElements, int elemSize, DataType dataType) {
		GLVBO *vbo = new GLVBO();

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;

		if(dataType == FLOAT) {
			vbo->dataType = GL_FLOAT;
		} else if(dataType == DOUBLE) {
			vbo->dataType = GL_DOUBLE;
		}

		vbo->drawType = GL_STATIC_DRAW_ARB;
		vbo->type = GL_ARRAY_BUFFER_ARB;

		glGenBuffersARB(1, &vbo->glID);							
		glBindBufferARB(vbo->type, vbo->glID);			
		glBufferDataARB(vbo->type, vbo->elementSize * vbo->numElements, data, vbo->drawType);
		glBindBufferARB(vbo->type, 0);

		return vbo;
	}

	//---------------------------------------------------------------------------
	//Desc:    creates vertex buffer object
	//Params:  data - pointer to data array, numElements - number of elements, elemSize - sizeof element, dataType - VBO data type
	//Returns: pointer to new GLVBO
	//---------------------------------------------------------------------------
	GLVBO *GLVBO::createIBO(void *data, int numElements, int elemSize, DataType dataType) {
		GLVBO *vbo = new GLVBO();

		vbo->numElements = numElements;
		vbo->elementSize = elemSize;

		if(dataType == UNSIGNED_INT) {
			vbo->dataType == GL_UNSIGNED_INT;
		} else if(dataType == UNSIGNED_SHORT) {
			vbo->dataType == GL_UNSIGNED_SHORT;
		}

		vbo->drawType = GL_STATIC_DRAW_ARB;
		vbo->type = GL_ELEMENT_ARRAY_BUFFER_ARB;

		glGenBuffersARB(1, &vbo->glID);							
		glBindBufferARB(vbo->type, vbo->glID);			
		glBufferDataARB(vbo->type, vbo->elementSize * vbo->numElements, data, vbo->drawType);
		glBindBufferARB(vbo->type, 0);

		return vbo;
	}

	//---------------------------------------------------------------------------
	//Desc:    GLVBO destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLVBO::~GLVBO() {
		glDeleteBuffersARB(1, &glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets VBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::set() {
		glBindBufferARB(type, glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets VBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::unset() {
		glBindBufferARB(type, 0);
	}


	//---------------------------------------------------------------------------
	//Desc:    sets VBO as vertex data source
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::setVertexSource(int numComp, int stride, int offset) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(numComp, dataType, stride, (void*)offset);
	}


	//---------------------------------------------------------------------------
	//Desc:    sets VBO as normal data source
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::setNormalSource(int stride, int offset) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(dataType, stride, (void*)offset);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets VBO as texture coords data source
	//Params:  tex_unit - number of texture unit
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::setTexCoordSource(int tex_unit, int numComp, int stride, int offset) {
		glClientActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(numComp, dataType, stride, (void*)offset);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets VBO as index data source
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::setIndexSource(int stride, int offset) {
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(dataType, stride, (void*)offset);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets vertex data source
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::unsetVertexSource() {
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets normal data source
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::unsetNormalSource() {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets tex coord data source
	//Params:  tex_unit - texture unit number
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::unsetTexCoordSource(int tex_unit) {
		glClientActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets index data source
	//Params:  vbo - pointer to VBO with data
	//Returns: -
	//---------------------------------------------------------------------------
	void GLVBO::unsetIndexSource() {
		glDisableClientState(GL_INDEX_ARRAY);
	}

}

