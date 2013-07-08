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
#include "MathLib.h"
#include "GLExtensions.h"
#include "GLTexture.h"
#include "GLVBO.h"
#include "GLShader.h"
#include "GLDisplayList.h"
#include "GLFBO.h"
#include "GLOcclusionQuery.h"
#include <vector>
//***************************************************************************

namespace NGEngine {
	class Engine;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main video system. Created one time
	//---------------------------------------------------------------------------
	class GLSystem:public IRender {
	//public:
	//	//---------------------------------------------------------------------------
	//	//Desc: Blending type enum
	//	//---------------------------------------------------------------------------
	//	enum BlendParam {
	//		ONE  = GL_ONE,
	//		ZERO = GL_ZERO,
	//		SRC_COLOR = GL_SRC_COLOR,
	//		DST_COLOR = GL_DST_COLOR,
	//		SRC_ALPHA = GL_SRC_ALPHA,
	//		DST_ALPHA = GL_DST_ALPHA,
	//		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	//		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	//		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	//		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	//	};

	//	//---------------------------------------------------------------------------
	//	//Desc: Depth function enum
	//	//---------------------------------------------------------------------------
	//	enum CompareType {
	//		NEVER    = GL_NEVER,
	//		LESS     = GL_LESS,
	//		EQUAL    = GL_EQUAL,
	//		LEQUAL   = GL_LEQUAL,
	//		GREATER  = GL_GREATER,
	//		NOTEQUAL = GL_NOTEQUAL,
	//		GEQUAL   = GL_GEQUAL,
	//		ALWAYS   = GL_ALWAYS,
	//	};

	//	//---------------------------------------------------------------------------
	//	//Desc: Cull type enum
	//	//---------------------------------------------------------------------------
	//	enum CullType {
	//		CCW = GL_CCW,
	//		CW  = GL_CW,
	//	};

	//	//---------------------------------------------------------------------------
	//	//Desc: Cull face
	//	//---------------------------------------------------------------------------
	//	enum CullFace {
	//		FRONT_FACE = GL_FRONT,
	//		BACK_FACE = GL_BACK,
	//	};

	//	//---------------------------------------------------------------------------
	//	//Desc: Buffers enum
	//	//---------------------------------------------------------------------------
	//	enum Buffer {
	//		COLOR_BUFFER = GL_COLOR_BUFFER_BIT,
	//		DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,
	//		STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT,
	//	};

	public:
		GLSystem();
		~GLSystem();

		String getVendor();
		String getRenderer();
		String getVersion(); 
		String getExtensions(); 

		int getNumTexUnits();
		int getMaxAniso();


		void requireExtension(const String &name);
		void initialise();

		void reshape(int width, int height);
		void getViewport(int *viewport);
		void clearColor(const Vec3 &color);
		void colorMask(bool r, bool g, bool b, bool a);
		void clear(GLbitfield buffers);
		void flush();
		void viewport(int x, int y);

		//----------Using-textures----
		void setColor(const Vec3 &color);
		void setColor(const Vec4 &color);

		//----------2D/3D-mode----
		void enable2d(bool normalized);
		void enable3d();
		void drawRect(float x0, float y0, float x3, float y3, float tx0, float ty0, float tx3, float ty3);

		//---Blending-------------------------------
		void blendFunc(BlendParam src, BlendParam dst);
		void enableBlending();
		void enableBlending(BlendParam src, BlendParam dst);
		void disableBlending();

		//---Alpha-test-----------------------------
		void alphaTestFunc(CompareType type, float alphaRef);
		void enableAlphaTest();
		void enableAlphaTest(CompareType type, float alphaRef);
		void disableAlphaTest();

		//---Z-Buffer-------------------------------
		void depthFunc(CompareType type);
		void enableDepth(CompareType type);
		void enableDepth();
		void disableDepth();
		void depthMask(bool mask);

		//---Scissor-test-------------------------------
		void scissorRect(int x, int y, int z, int w);
		void enableScissor(int x, int y, int z, int w);
		void enableScissor();
		void disableScissor();

		//---Z-Buffer-------------------------------
		void polygonOffsetFill(float a, float b);
		void enablePolygonOffsetFill(float a, float b);
		void enablePolygonOffsetFill();
		void disablePolygonOffsetFill();

		//---Culling-------------------------------
		void cullFunc(CullType type);
		void cullFace(CullFace face);
		void enableCulling(CullType type);
		void enableCulling(CullFace face);
		void enableCulling();
		void disableCulling();

		//---Clip-plains--------------------------
		void clipPlane(const Vec4 &plain, int plainNum);
		void enableClipPlane(int plainNum);
		void enableClipPlane(const Vec4 &plain, int plainNum);
		void disableClipPlane(int plainNum);

		//---Matrix-works-------------------------
		void setMatrixMode_Projection();
		void setMatrixMode_Modelview();

		void pop();
		void push();
		void identity();

		Mat4 getMatrix_MVP();
		Mat4 getMatrix_Projection();
		Mat4 getMatrix_Modelview();

		void multMatrix(const Mat4 &matrix);
		void loadMatrix(const Mat4 &matrix);

		void translate(const Vec3 &pos);
		void rotate(float angle, const Vec3 &axis);
		void scale(const Vec3 &coef);

		//---Draw-Geom-Stream---------------------
		void drawIndexedGeometry(void *indices, int indexCount);
		void drawGeometry(int vertexCount);

	private:
		friend class GLTexture;
		friend class GLVBO;
		friend class GLShader;
		friend class GLDisplayList;
	};

};