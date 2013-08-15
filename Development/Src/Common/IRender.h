/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    IRender.h
* Desc:    Render interface.
* Version: 1.0
* Author:  Nick Galko <galek@vegaengine.com>
*
* This file is part of the Vega engine (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/
#ifndef __I_RENDER__H
#define __I_RENDER__H

#include <string>

namespace VEGA {
	class Mat4;
	class Vec3;
	class Vec4;
	
	struct ITexture{
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
	};

	struct IRender{
		//Nick:TODO:Здесь значения для enum'ов взяты из GL рендера
		//---------------------------------------------------------------------------
		//Desc: Blending type enum
		//---------------------------------------------------------------------------
		enum BlendParam {
			ONE  = 1,
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
			NEVER    = 0x0209,
			LESS     = 0x0201,
			EQUAL    = 0x0202,
			LEQUAL   = 0x0203,
			GREATER  = 0x0204,
			NOTEQUAL = 0x0205,
			GEQUAL   = 0x0206,
			ALWAYS   = 0x0207,
		};

		//---------------------------------------------------------------------------
		//Desc: Cull type enum
		//---------------------------------------------------------------------------
		enum CullType {
			CCW = 0x0901,
			CW  = 0x0900,
		};

		//---------------------------------------------------------------------------
		//Desc: Cull face
		//---------------------------------------------------------------------------
		enum CullFace {
			FRONT_FACE = 0x0404,
			BACK_FACE = 0x0405,
		};

		//---------------------------------------------------------------------------
		//Desc: Buffers enum
		//---------------------------------------------------------------------------
		enum Buffer {
			COLOR_BUFFER = 0x00004000,
			DEPTH_BUFFER = 0x00000100,
			STENCIL_BUFFER = 0x00000400,
		};
		
		virtual void  requireExtension(const std::string &name,bool _fatal=false)=0;
		virtual void  initialise()=0;
		
		

		unsigned int currTextureTarget;
		int currTextureUnit;
		ITexture::Filter defFilter;
		ITexture::Aniso defAniso;

		virtual void  reshape(int width, int height)=0;
		virtual void  getViewport(int *viewport)=0;
		virtual void  clearColor(const Vec3 &color)=0;
		virtual void  colorMask(bool r, bool g, bool b, bool a)=0;
		virtual void  clear(unsigned int buffers)=0;
		virtual void  flush()=0;
		virtual void  viewport(int x, int y)=0;

		//----------Using-textures----
		virtual void  setColor(const Vec3 &color)=0;
		virtual void  setColor(const Vec4 &color)=0;

		//----------2D/3D-mode----
		virtual void  enable2d(bool normalized)=0;
		virtual void  enable3d()=0;
		virtual void  drawRect(float x0, float y0, float x3, float y3, float tx0, float ty0, float tx3, float ty3)=0;

		//---Blending-------------------------------
		virtual void  blendFunc(BlendParam src, BlendParam dst)=0;
		virtual void  enableBlending()=0;
		virtual void  enableBlending(BlendParam src, BlendParam dst)=0;
		virtual void  disableBlending()=0;

		//---Alpha-test-----------------------------
		virtual void  alphaTestFunc(CompareType type, float alphaRef)=0;
		virtual void  enableAlphaTest()=0;
		virtual void  enableAlphaTest(CompareType type, float alphaRef)=0;
		virtual void  disableAlphaTest()=0;

		//---Z-Buffer-------------------------------
		virtual void  depthFunc(CompareType type)=0;
		virtual void  enableDepth(CompareType type)=0;
		virtual void  enableDepth()=0;
		virtual void  disableDepth()=0;
		virtual void  depthMask(bool mask)=0;

		//---Scissor-test-------------------------------
		virtual void scissorRect(int x, int y, int z, int w)=0;
		virtual void enableScissor(int x, int y, int z, int w)=0;
		virtual void enableScissor()=0;
		virtual void disableScissor()=0;

		//---Z-Buffer-------------------------------
		virtual void  polygonOffsetFill(float a, float b)=0;
		virtual void  enablePolygonOffsetFill(float a, float b)=0;
		virtual void  enablePolygonOffsetFill()=0;
		virtual void  disablePolygonOffsetFill()=0;

		//---Culling-------------------------------
		virtual void  cullFunc(CullType type)=0;
		virtual void  cullFace(CullFace face)=0;
		virtual void  enableCulling(CullType type)=0;
		virtual void  enableCulling(CullFace face)=0;
		virtual void  enableCulling()=0;
		virtual void  disableCulling()=0;

		//---Clip-plains--------------------------
		virtual void  clipPlane(const Vec4 &plain, int plainNum)=0;
		virtual void  enableClipPlane(int plainNum)=0;
		virtual void  enableClipPlane(const Vec4 &plain, int plainNum)=0;
		virtual void  disableClipPlane(int plainNum)=0;


		//---Matrix-works-------------------------
		virtual void setMatrixMode_Projection()=0;
		virtual void setMatrixMode_Modelview()=0;

		virtual void pop()=0;
		virtual void push()=0;
		virtual void identity()=0;

		virtual Mat4 getMatrix_MVP()=0;
		virtual Mat4 getMatrix_Projection()=0;
		virtual Mat4 getMatrix_Modelview()=0;

		virtual void multMatrix(const Mat4 &matrix)=0;
		virtual void loadMatrix(const Mat4 &matrix)=0;

		virtual void translate(const Vec3 &pos)=0;
		virtual void rotate(float angle, const Vec3 &axis)=0;
		virtual void scale(const Vec3 &coef)=0;

		//---Draw-Geom-Stream---------------------
		virtual void drawIndexedGeometry(void *indices, int indexCount)=0;
		virtual void drawGeometry(int vertexCount)=0;

	};
}
#endif