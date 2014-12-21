/* Copyright (C) 2006-2015, NG Games Ltd. All rights reserved.
*
* File:    IRender.h
* Desc:    Render interface.
* Version: 1.0
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/
#ifndef __I_RENDER__H
#define __I_RENDER__H

#include "StringHelper.h"

namespace NGTech {
	class Mat4;
	class Vec2;
	class Vec3;
	class Vec4;
	class I_ILImage;
	struct I_DisplayList;
	struct I_OcclusionQuery;
	struct I_FBOManager;
	struct I_VBManager;
	struct I_Shader;
	struct I_Texture;

	struct I_Texture
	{
		virtual ~I_Texture(){}
		/**
		Texture target enum
		*/
		enum Target {
			TEXTURE_2D = 0x0DE1,
			TEXTURE_3D = 0x806F,
			TEXTURE_CUBE = 0x8513,
		};

		/**
		Texture edge wrap enum
		*/
		enum Wrap {
			REPEAT = 0x2901,
			CLAMP = 0x2900,
			CLAMP_TO_EDGE = 0x812F,
		};
		/**
		Texture src format enum
		*/
		enum Format {
			RGB,
			RGBA,

			RGB_16,
			RGBA_16,

			RGB_FP16,
			RGBA_FP16,

			RGB_FP32,
			RGBA_FP32,
		};


		/**
		Texture filter enum
		*/
		enum Filter {
			NEAREST,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			LINEAR_MIPMAP_LINEAR,
		};

		//---------------------------------------------------------------------------
		//Desc: Texture anisotropy enum
		//---------------------------------------------------------------------------
		enum Aniso {
			ANISO_X0 = 0,
			ANISO_X1 = 1,
			ANISO_X2 = 2,
			ANISO_X4 = 4,
			ANISO_X8 = 8,
			ANISO_X16 = 16,
		};
		int width, height, depth;

	public:
		virtual void setWrap(Wrap wrap) = 0;
		virtual void setFilter(Filter filter) = 0;
		virtual void setAniso(Aniso aniso) = 0;

		virtual void  Set() = 0;
		virtual void  UnSet() = 0;

		virtual void set(size_t tex_unit) = 0;
		virtual void unset(size_t tex_unit) = 0;

		virtual void beginRenderTo() = 0;
		virtual void copy(int face = -1) = 0;
		virtual void endRenderTo() = 0;
	public:
		unsigned int glID;

		unsigned int target;

		unsigned int minFilter;
		unsigned int magFilter;

		unsigned int aniso;
		unsigned int wrap;

		unsigned int internalFormat;
		unsigned int srcFormat;
		unsigned int dataType;
	};

	struct I_Render{
		/**
		*/
		virtual ~I_Render(){}
		/**
		*/
		virtual I_DisplayList* GetDL() = 0;
		/**
		*/
		virtual I_OcclusionQuery* GetOQ() = 0;
		/**
		*/
		virtual I_Texture* TextureCreate2D(const String &path) = 0;
		/**
		*/
		virtual I_Texture* TextureCreateCube(const String &path) = 0;
		/**
		*/
		virtual I_Texture *TextureCreate2D(I_ILImage *image) = 0;
		/**
		*/
		virtual I_Texture *TextureCreate3D(I_ILImage *image) = 0;
		/**
		*/
		virtual I_Texture *TextureCreateCube(I_ILImage **image) = 0;
		/**
		*/
		virtual I_Texture *TextureCreate2D(int width, int height, int format) = 0;
		/**
		*/
		virtual I_Texture *TextureCreate3D(int width, int height, int depth, int format) = 0;
		/**
		*/
		virtual I_Texture *TextureCreateCube(int width, int height, int format) = 0;
		/**
		*/
		virtual I_Shader *ShaderCreate(const String &path, const String &defines = "") = 0;
		/**
		*/
		virtual I_FBOManager*CreateFBO(int x, int y) = 0;
		/**
		*/
		virtual I_VBManager *CreateIBO(void *data, int numElements, int elemSize, int dataType) = 0;
		/**
		*/
		virtual I_VBManager *CreateVBO(void *data, int numElements, int elemSize, int dataType, int drawType) = 0;
		/**
		*/
		virtual I_ILImage* CreateImage2D(const String &path) = 0;
		/**
		*/
		virtual I_ILImage* CreateImageEmpty2D(int width, int height, int format) = 0;
		/**
		*/
		virtual I_ILImage* CreateImageNoise2D(int width, int height, int format) = 0;
		/**
		*/
		virtual I_ILImage* CreateImageEmpty3D(int width, int height, int depth, int format) = 0;
		/**
		*/
		virtual I_ILImage* CreateImageNoise3D(int width, int height, int depth, int format) = 0;
		/**
		*/
		virtual void* GetGUIImageLoader() = 0;
		/**
		*/
		//Nick:TODO:Здесь значения для enum'ов взяты из GL рендера
		//---------------------------------------------------------------------------
		//Desc: Blending type enum
		//---------------------------------------------------------------------------
		enum BlendParam {
			ONE = 1,
			ZERO = 0,
			SRC_COLOR = 0x0300,
			DST_COLOR = 0x0306,
			SRC_ALPHA = 0x0302,
			DST_ALPHA = 0x0304,
			ONE_MINUS_SRC_COLOR = 0x0301,
			ONE_MINUS_DST_COLOR = 0x0307,
			ONE_MINUS_SRC_ALPHA = 0x0303,
			ONE_MINUS_DST_ALPHA = 0x0305,
		};
		//---------------------------------------------------------------------------
		//Desc: Depth function enum
		//---------------------------------------------------------------------------
		enum CompareType {
			NEVER = 0x0209,
			LESS = 0x0201,
			EQUAL = 0x0202,
			LEQUAL = 0x0203,
			GREATER = 0x0204,
			NOTEQUAL = 0x0205,
			GEQUAL = 0x0206,
			ALWAYS = 0x0207,
		};
		//---------------------------------------------------------------------------
		//Desc: Cull type enum
		//---------------------------------------------------------------------------
		enum CullType {
			CULL_CCW = 0,
			CULL_CW,
			CULL_FRONT,
			NUM_FRONT_MODES
		};
		//---------------------------------------------------------------------------
		//Desc: Cull face
		//---------------------------------------------------------------------------
		enum CullFace {
			CULL_NONE = 0,
			CULL_FRONT_FACE,
			CULL_BACK_FACE,
			NUM_CULL_MODES,
		};
		//---------------------------------------------------------------------------
		//Desc: Buffers enum
		//---------------------------------------------------------------------------
		enum Buffer {
			COLOR_BUFFER = 0x00004000,
			DEPTH_BUFFER = 0x00000100,
			STENCIL_BUFFER = 0x00000400,
		};
		/**
		*/
		virtual bool  requireExtension(const std::string &name, bool _fatal = false) = 0;
		/**
		*/
		virtual void  initialise() = 0;
		/**
		*/
		virtual void  reshape(int width, int height) = 0;
		/**
		*/
		virtual void  getViewport(int *viewport) = 0;
		/**
		*/
		virtual void  clearColor(const Vec3 &color) = 0;
		/**
		*/
		virtual void  colorMask(bool r, bool g, bool b, bool a) = 0;
		/**
		*/
		virtual void  clear(unsigned int buffers) = 0;
		/**
		*/
		virtual void  flush() = 0;
		/**
		*/
		virtual void  viewport(int x, int y) = 0;
		/**
		*/
		virtual void  endFrame(){ swapBuffers(); }
		/**
		Using-textures
		*/
		virtual void  setColor(const Vec3 &color) = 0;
		/**
		*/
		virtual void  setColor(const Vec4 &color) = 0;
		/**
		2D/3D-mode
		*/
		virtual void  enable2d(bool normalized) = 0;
		/**
		*/
		virtual void  enable3d() = 0;
		/**
		*/
		virtual void  drawRect(float x0, float y0, float x3, float y3, float tx0, float ty0, float tx3, float ty3) = 0;
		/**
		Blendig
		*/
		virtual void  blendFunc(BlendParam src, BlendParam dst) = 0;
		/**
		*/
		virtual void  enableBlending() = 0;
		/**
		*/
		virtual void  enableBlending(BlendParam src, BlendParam dst) = 0;
		/**
		*/
		virtual void  disableBlending() = 0;
		/**
		Alpha-test
		*/
		virtual void  alphaTestFunc(CompareType type, float alphaRef) = 0;
		/**
		*/
		virtual void  enableAlphaTest() = 0;
		/**
		*/
		virtual void  enableAlphaTest(CompareType type, float alphaRef) = 0;
		/**
		*/
		virtual void  disableAlphaTest() = 0;
		/**
		Z-Buffer
		*/
		virtual void  depthFunc(CompareType type) = 0;
		/**
		*/
		virtual void  enableDepth(CompareType type) = 0;
		/**
		*/
		virtual void  enableDepth() = 0;
		/**
		*/
		virtual void  disableDepth() = 0;
		/**
		*/
		virtual void  depthMask(bool mask) = 0;
		/**
		Scissor-test
		*/
		virtual void scissorRect(int x, int y, int z, int w) = 0;
		/**
		*/
		virtual void enableScissor(int x, int y, int z, int w) = 0;
		/**
		*/
		virtual void enableScissor() = 0;
		/**
		*/
		virtual void disableScissor() = 0;
		/**
		Z-Buffer
		*/
		virtual void  polygonOffsetFill(float a, float b) = 0;
		/**
		*/
		virtual void  enablePolygonOffsetFill(float a, float b) = 0;
		/**
		*/
		virtual void  enablePolygonOffsetFill() = 0;
		/**
		*/
		virtual void  disablePolygonOffsetFill() = 0;
		/**
		Culling
		*/
		virtual void  SetPolygonFront(CullType type) = 0;
		/**
		*/
		virtual void  SetPolygonCull(CullFace face) = 0;
		/**
		*/
		virtual void  enableCulling(CullType type) = 0;
		/**
		*/
		virtual void  enableCulling(CullFace face) = 0;
		/**
		*/
		virtual void  enableCulling() = 0;
		/**
		*/
		virtual void  disableCulling() = 0;
		/**
		Clip-plains
		*/
		virtual void  clipPlane(const Vec4 &plain, int plainNum) = 0;
		/**
		*/
		virtual void  enableClipPlane(int plainNum) = 0;
		/**
		*/
		virtual void  enableClipPlane(const Vec4 &plain, int plainNum) = 0;
		/**
		*/
		virtual void  disableClipPlane(int plainNum) = 0;
		/**
		Matrix-works
		*/
		virtual void setMatrixMode_Projection() = 0;
		/**
		*/
		virtual void setMatrixMode_Modelview() = 0;
		/**
		*/
		virtual void pop() = 0;
		/**
		*/
		virtual void push() = 0;
		/**
		*/
		virtual void identity() = 0;
		/**
		*/
		virtual Mat4 getMatrix_MVP() = 0;
		/**
		*/
		virtual Mat4 getMatrix_Projection() = 0;
		/**
		*/
		virtual Mat4 getMatrix_Modelview() = 0;
		/**
		*/
		virtual void multMatrix(const Mat4 &matrix) = 0;
		/**
		*/
		virtual void loadMatrix(const Mat4 &matrix) = 0;
		/**
		*/
		virtual void translate(const Vec3 &pos) = 0;
		/**
		*/
		virtual void rotate(float angle, const Vec3 &axis) = 0;
		/**
		*/
		virtual void scale(const Vec3 &coef) = 0;
		/**
		Draw-Geom-Stream
		*/
		virtual void drawIndexedGeometry(void *indices, int indexCount) = 0;
		/**
		*/
		virtual void drawGeometry(int vertexCount) = 0;
		/**
		OpenGL Low Level
		*/
		virtual bool createContext(I_Window*) { return false; }
		/**
		*/
		virtual void swapBuffers() = 0;
		/**
		*/
	public:
		/**
		*/
		unsigned int currTextureTarget;
		/**
		*/
		int currTextureUnit;
		/**
		*/
		I_Texture::Filter defFilter;
		/**
		*/
		I_Texture::Aniso defAniso;
	};

	struct I_Shader
	{
		I_Shader(){}

		/**
		*/
		virtual ~I_Shader(){}

		/**
		*/
		virtual void set() = 0;

		/**
		*/
		virtual void unset() = 0;

		/**
		Release shader
		*/
		virtual void Release() = 0;

		/**
		*/
		virtual void sendMat4(const String &name, const Mat4 &value) = 0;

		/**
		*/
		virtual void sendVec4(const String &name, const Vec4 &value) = 0;

		/**
		*/
		virtual void sendVec3(const String &name, const Vec3 &value) = 0;

		/**
		*/
		virtual void sendVec2(const String &name, const Vec2 &value) = 0;

		/**
		*/
		virtual void sendFloat(const String &name, float value) = 0;

		/**
		*/
		virtual void sendInt(const String &name, size_t value) = 0;

		/**
		*/
		virtual int GetUniformLocation(const char*_loc, bool isOptional) = 0;

		/**
		*/
		virtual int GetAttribLocation(const char* attribute, bool isOptional) = 0;

		/**
		*/
		virtual void BindTexture2D(const char *name, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual void BindTexture2D(int index, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual void BindTextureRect(const char *name, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual bool CreateShader(const String &path, const String &defines, bool _save) = 0;

		/**
		*/
		virtual void BindTextureRect(int index, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual void BindTextureArray(const char *name, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual void BindTextureArray(int index, int unit, unsigned int tex) = 0;

		/**
		*/
		virtual void SetUniform1i(const char *name, int value) = 0;

		/**
		*/
		virtual void SetUniform1i(int index, int value) = 0;

		/**
		*/
		virtual void SetUniform2i(const char *name, int x, int y) = 0;

		/**
		*/
		virtual void SetUniform2i(int index, int x, int y) = 0;

		/**
		*/
		virtual void SetUniform3i(const char *name, int x, int y, int z) = 0;

		/**
		*/
		virtual void SetUniform3i(int index, int x, int y, int z) = 0;

		/**
		*/
		virtual void SetUniform1f(const char *name, float value) = 0;

		/**
		*/
		virtual void SetUniform1f(int index, float value) = 0;

		/**
		*/
		virtual void SetUniform2f(const char *name, float x, float y) = 0;

		/**
		*/
		virtual void SetUniform2f(int index, float x, float y) = 0;

		/**
		*/
		virtual void SetUniform3f(const char *name, float x, float y, float z) = 0;

		/**
		*/
		virtual void SetUniform3f(int index, float x, float y, float z) = 0;

		/**
		*/
		virtual void SetUniform4f(const char *name, float x, float y, float z, float w) = 0;

		/**
		*/
		virtual void SetUniform4f(int index, float x, float y, float z, float w) = 0;

		/**
		*/
		virtual void SetUniform3fv(const char *name, const float *value, int count) = 0;

		/**
		*/
		virtual void SetUniform3fv(int index, const float *value, int count) = 0;

		/**
		*/
		virtual void SetUniform4fv(const char *name, const float *value, int count) = 0;

		/**
		*/
		virtual void SetUniform4fv(int index, const float *value, int count) = 0;

		/**
		*/
		virtual void SetUniformMatrix4fv(const char *name, float *m, int count, bool transpose) = 0;

		/**
		*/
		virtual void SetUniformMatrix4fv(int index, float *m, int count, bool transpose) = 0;

		/**
		*/
		virtual void AddAttribute(const std::string& attribute) = 0;

		/**
		*/
		virtual void AddUniform(const std::string& uniform) = 0;
		/**
		An indexer that returns the location of the attribute/uniform
		*/
		unsigned int operator[](const std::string& attribute);
		/**
		*/
		unsigned int operator()(const std::string& uniform);
		/**
		An indexer that returns the location of the attribute/uniform
		*/
		unsigned int operator[](const char* attribute);
		/**
		*/
		unsigned int operator()(const char* uniform);
	};


	struct I_VBManager
	{
		//---------------------------------------------------------------------------
		//Desc: VBO data type
		//---------------------------------------------------------------------------
		enum DataType {
			FLOAT,
			DOUBLE,
			UNSIGNED_INT,
			UNSIGNED_SHORT,
		};

		enum TypeDraw
		{
			STATIC,
			STREAM
		};
		/**
		*/
		virtual ~I_VBManager(){}
		/**
		*/
		virtual void setVertexSource(int numComp, int stride, int offset) = 0;
		/**
		*/
		virtual void setNormalSource(int stride, int offset) = 0;
		/**
		*/
		virtual void setTexCoordSource(int tex_unit, int numComp, int stride, int offset) = 0;
		/**
		*/
		virtual void setIndexSource(int stride, int offset) = 0;
		/**
		*/
		virtual void unsetVertexSource() = 0;
		/**
		*/
		virtual void unsetNormalSource() = 0;
		/**
		*/
		virtual void unsetTexCoordSource(int tex_unit) = 0;
		/**
		*/
		virtual void unsetIndexSource() = 0;
		/**
		*/
		virtual void set() = 0;
		/**
		*/
		virtual void unset() = 0;
		/**
		*/
		virtual void* map(int offset = 0, void** data = nullptr) = 0;
		/**
		*/
		virtual void unMap() = 0;
		/**
		*/
	protected:
		unsigned int glID;
		int numElements;
		int elementSize;

		unsigned int dataType;
		unsigned int type;
		unsigned int drawType;

		unsigned int gs, vs, fs, cs, tcs, tes, program;
	};

	struct I_OcclusionQuery
	{
		/**
		*/
		virtual ~I_OcclusionQuery(){}

		/**
		Begins rendering to query
		*/
		virtual void beginRendering() = 0;

		/**
		End rendering to query
		*/
		virtual void endRendering() = 0;

		/**
		Get number of passed samples
		\return number of passed samples
		*/
		virtual unsigned int getResult() = 0;

		/**
		clear query
		*/
		virtual void clear() = 0;

		/** render destroy
		*/
		virtual void destroy() = 0;

	protected:
		unsigned int query_id;
		unsigned int result;
	};

	struct I_DisplayList
	{
	public:
		/**
		*/
		virtual ~I_DisplayList(){}
		/**
		Begins the display list build
		*/
		virtual void beginBuild() = 0;

		/**
		Ends the display list build
		*/
		virtual void endBuild() = 0;

		/**
		Calls the display list
		*/
		virtual void call() = 0;

	protected:
		unsigned int glID;
	};

	struct I_FBOManager{
	public:
		/**
		*/
		virtual ~I_FBOManager(){}
		/**
		*/
		virtual void createColorAttachment() = 0;
		/**
		*/
		virtual void createDepthAttachment() = 0;
		/**
		*/
		virtual void createStencilAttachment() = 0;
		/**
		*/
		virtual void setColorTarget(I_Texture *texture = NULL, int face = -1) = 0;
		/**
		*/
		virtual void setDepthTarget(I_Texture *texture = NULL) = 0;
		/**
		*/
		virtual void set() = 0;
		/**
		*/
		virtual void unset() = 0;
		/**
		*/
		virtual void clear() = 0;
		/**
		*/
		virtual void flush() = 0;
		/**
		*/
	protected:
		unsigned int glID;

		unsigned int glColorID;
		unsigned int glStencilID;
		unsigned int glDepthID;

		I_Texture *colorTarget;
		I_Texture *depthTarget;

		int width, height;
	};
}
#endif