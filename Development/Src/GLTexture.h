/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once 

//***************************************************************************
#include "Core/IncludesAndLibs.h"
//***************************************************************************
#include "ILImage.h"
//***************************************************************************

namespace VEGA {

//---------------------------------------------------------------------------
//Desc: Texture class
//---------------------------------------------------------------------------
class GLTexture:public ITexture {
public:
	//---------------------------------------------------------------------------
	//Desc: Texture target enum
	//---------------------------------------------------------------------------
	enum Target {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_3D = GL_TEXTURE_3D_EXT,
		TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP_ARB,
	};

	//---------------------------------------------------------------------------
	//Desc: Texture edge wrap enum
	//---------------------------------------------------------------------------
	enum Wrap {
		REPEAT = GL_REPEAT,
		CLAMP = GL_CLAMP,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	};

public:
	static GLTexture *create2d(const String &path);
	static GLTexture *createCube(const String &path);
	
	static GLTexture *create2d(ILImage *image);
	static GLTexture *create3d(ILImage *image);
	static GLTexture *createCube(ILImage **image);

	static GLTexture *create2d(int width, int height, Format format);
	static GLTexture *create3d(int width, int height, int depth, Format format);
	static GLTexture *createCube(int width, int height, Format format);
	
	~GLTexture();

	void setWrap(Wrap wrap);
	void setFilter(Filter filter);
	void setAniso(Aniso aniso);

	void set(int tex_unit);
	void unset(int tex_unit);

	void beginRenderTo();
	void copy(int face = -1);
	void endRenderTo();

private:
	static GLTexture *create(int width, int height, int depth, Target target, Format format, void **data);
	
	GLuint glID;

	GLuint target;
	
	GLuint minFilter;
	GLuint magFilter;

	GLuint aniso;
	GLuint wrap;

	GLuint internalFormat;
	GLuint srcFormat;
	GLenum dataType;

	friend class GLFBO;
};

};