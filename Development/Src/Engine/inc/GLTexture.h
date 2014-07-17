/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "../Common/IRender.h"
#include "IncludesAndLibs.h"
//***************************************************************************
#include "ILImage.h"
//***************************************************************************

namespace NGTech {

	/**
	Texture class
	*/
	class GLTexture :public I_Texture {
	public:
		~GLTexture();

		void setWrap(Wrap wrap);
		void setFilter(Filter filter);
		void setAniso(Aniso aniso);

		void set(int tex_unit);
		void unset(int tex_unit);

		void beginRenderTo();
		void copy(int face = -1);
		void endRenderTo();
	private://Only For friends
		static GLTexture *create(int width, int height, int depth, Target target, Format format, void **data);
		static GLTexture *create2d(const String &path);
		static GLTexture *createCube(const String &path);

		static GLTexture *create2d(int width, int height, Format format);
		static GLTexture *create3d(int width, int height, int depth, Format format);
		static GLTexture *createCube(int width, int height, Format format);

		static GLTexture *create2d(I_ILImage *image);
		static GLTexture *create3d(I_ILImage *image);
		static GLTexture *createCube(I_ILImage **image);

	private:
		friend class GLFBO;
		friend class GLSystem;
	};
}