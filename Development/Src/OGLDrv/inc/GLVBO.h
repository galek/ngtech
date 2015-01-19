#pragma once 

//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: Vertex buffer object class
	//---------------------------------------------------------------------------
	class GLVBO :public I_VBManager
	{
	private:
		GLVBO(const GLVBO& other) = delete;
		GLVBO& operator=(const GLVBO& other) = delete;
	public:
		enum BUFType
		{
			BUF_VERTEX = 0,
			BUF_INDEX
		};
	public:
		GLVBO();
		static GLVBO *createIBO(void *data, int numElements, int elemSize, DataType dataType);
		static GLVBO *createVBO(void *data, int numElements, int elemSize, DataType dataType, TypeDraw drawType);
		virtual ~GLVBO();

		// устанавливаем созданный VBO как текущий
		virtual void Bind() const;
		virtual void UnBind() const;
		virtual void BindIndex(unsigned int idx) const;
		virtual void UnbindIndex(unsigned int idx) const;
		virtual void Allocate(const void *data, size_t size, TypeDraw usage);

		virtual void setVertexSource(int numComp, int stride, int offset);
		virtual void setNormalSource(int stride, int offset);
		virtual void setTexCoordSource(int tex_unit, int numComp, int stride, int offset);
		virtual void setIndexSource(int stride, int offset);

		virtual void unsetVertexSource();
		virtual void unsetNormalSource();
		virtual void unsetTexCoordSource(int tex_unit);
		virtual void unsetIndexSource();

		virtual void FillBuffer(size_t offset);

		virtual void *map(int offset = 0, void** data = nullptr);
		virtual void unMap();
		void SetType(BUFType _b)
		{
			if (_b == BUF_INDEX)
				type = GL_ELEMENT_ARRAY_BUFFER;
			else if (_b == BUF_VERTEX)
				type = GL_ARRAY_BUFFER;
		}
		ENGINE_INLINE unsigned int GetType() { return type; }
		ENGINE_INLINE unsigned int GetID() { return glID; }
	public:
		// создадим Vertex Buffer Object (VBO)
		virtual void _Create();
	private:
		BUFType mBUFType;

		struct locked_data
		{
			void* ptr;
			GLuint flags;
		};
		locked_data	vertexdata_locked;
		locked_data	indexdata_locked;
		size_t _size;
	};
}