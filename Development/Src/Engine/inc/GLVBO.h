/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

//---------------------------------------------------------------------------
//Desc: Vertex buffer object class
//---------------------------------------------------------------------------
class GLVBO {
public:
	//---------------------------------------------------------------------------
	//Desc: VBO data type
	//---------------------------------------------------------------------------
	enum DataType {
		FLOAT,
		DOUBLE,
		UNSIGNED_INT,
		UNSIGNED_SHORT,
	};

public:
	static GLVBO *createIBO(void *data, int numElements, int elemSize, DataType dataType);
	static GLVBO *createVBO(void *data, int numElements, int elemSize, DataType dataType);
	~GLVBO();

	void setVertexSource(int numComp, int stride, int offset);
	void setNormalSource(int stride, int offset);
	void setTexCoordSource(int tex_unit, int numComp, int stride, int offset);
	void setIndexSource(int stride, int offset);

	void unsetVertexSource();
	void unsetNormalSource();
	void unsetTexCoordSource(int tex_unit);
	void unsetIndexSource();

	void set();
	void unset();

private:
	GLuint glID;
	int numElements;
	int elementSize;
	
	GLenum dataType;
	GLuint type;
	GLuint drawType;
};

};