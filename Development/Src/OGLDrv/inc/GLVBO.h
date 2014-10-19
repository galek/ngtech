#pragma once 

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
		static GLVBO *createVBO(void *data, int numElements, int elemSize, DataType dataType, TypeDraw drawType);
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

		virtual void *map(int offset = 0);
		virtual unsigned char unMap();
	};
}