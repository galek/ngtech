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
	class GLVBO :public I_VBManager {
	public:
		static GLVBO *createIBO(void *data, int numElements, int elemSize, DataType dataType);
		static GLVBO *createVBO(void *data, int numElements, int elemSize, DataType dataType);
		virtual ~GLVBO();

		virtual void setVertexSource(int numComp, int stride, int offset);
		virtual void setNormalSource(int stride, int offset);
		virtual void setTexCoordSource(int tex_unit, int numComp, int stride, int offset);
		virtual void setIndexSource(int stride, int offset);

		virtual void unsetVertexSource();
		virtual void unsetNormalSource();
		virtual void unsetTexCoordSource(int tex_unit);
		virtual void unsetIndexSource();

		virtual void set();
		virtual void unset();
	};
}