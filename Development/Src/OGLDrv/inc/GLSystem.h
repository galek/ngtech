#pragma once 

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
#include "GLExtensions.h"
#include "GLTexture.h"
#include "GLVBO.h"
#include "GLShader.h"
#include "GLDisplayList.h"
#include "GLFBO.h"
#include "GLOcclusionQuery.h"
#include "Renderdll.h"
#include <vector>
//***************************************************************************

namespace NGTech {
	class Engine;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main video system. Created one time
	//---------------------------------------------------------------------------
	class RENDER_API GLSystem :public I_Render {
	public:
		virtual I_DisplayList* GetDL();
		virtual I_OcclusionQuery* GetOQ();


		virtual I_Texture* TextureCreate2D(const String &path);
		virtual I_Texture* TextureCreateCube(const String &path);
		virtual I_Texture *TextureCreate2D(int width, int height, int format);
		virtual I_Texture *TextureCreate3D(int width, int height, int depth, int format);
		virtual I_Texture *TextureCreateCube(int width, int height, int format);
		virtual I_Texture *TextureCreate2D(I_ILImage *image);
		virtual I_Texture *TextureCreate3D(I_ILImage *image);
		virtual I_Texture *TextureCreateCube(I_ILImage **image);

		virtual I_Shader  *ShaderCreate(const String &path, const String &defines = "");
		virtual I_VBManager *CreateIBO(void *data, int numElements, int elemSize, int dataType);
		virtual I_VBManager *CreateVBO(void *data, int numElements, int elemSize, int dataType);
		virtual I_FBOManager*CreateFBO(int x, int y);


		virtual I_ILImage* CreateImage2D(const String &path);
		virtual I_ILImage* CreateImageEmpty2D(int width, int height, int format);
		virtual I_ILImage* CreateImageNoise2D(int width, int height, int format);
		virtual I_ILImage* CreateImageEmpty3D(int width, int height, int depth, int format);
		virtual I_ILImage* CreateImageNoise3D(int width, int height, int depth, int format);
	public:
		GLSystem();
		~GLSystem();

		String getVendor();
		String getRenderer();
		String getVersion();
		String getExtensions();

		virtual int getNumTexUnits();
		virtual int getMaxAniso();


		virtual void requireExtension(const String &name, bool _fatal = false);
		virtual void initialise();

		virtual void reshape(int width, int height);
		virtual void getViewport(int *viewport);
		virtual void clearColor(const Vec3 &color);
		virtual void colorMask(bool r, bool g, bool b, bool a);
		virtual void clear(GLbitfield buffers);
		virtual void flush();
		virtual void viewport(int x, int y);

		//----------Using-textures----
		virtual void setColor(const Vec3 &color);
		virtual void setColor(const Vec4 &color);

		//----------2D/3D-mode----
		virtual void enable2d(bool normalized);
		virtual void enable3d();
		virtual void drawRect(float x0, float y0, float x3, float y3, float tx0, float ty0, float tx3, float ty3);

		//---Blending-------------------------------
		virtual void blendFunc(BlendParam src, BlendParam dst);
		virtual void enableBlending();
		virtual void enableBlending(BlendParam src, BlendParam dst);
		virtual void disableBlending();

		//---Alpha-test-----------------------------
		virtual void alphaTestFunc(CompareType type, float alphaRef);
		virtual void enableAlphaTest();
		virtual void enableAlphaTest(CompareType type, float alphaRef);
		virtual void disableAlphaTest();

		//---Z-Buffer-------------------------------
		virtual void depthFunc(CompareType type);
		virtual void enableDepth(CompareType type);
		virtual void enableDepth();
		virtual void disableDepth();
		virtual void depthMask(bool mask);

		//---Scissor-test-------------------------------
		virtual void scissorRect(int x, int y, int z, int w);
		virtual void enableScissor(int x, int y, int z, int w);
		virtual void enableScissor();
		virtual void disableScissor();

		//---Z-Buffer-------------------------------
		virtual void polygonOffsetFill(float a, float b);
		virtual void enablePolygonOffsetFill(float a, float b);
		virtual void enablePolygonOffsetFill();
		virtual void disablePolygonOffsetFill();

		//---Culling-------------------------------
		virtual void cullFunc(CullType type);
		virtual void cullFace(CullFace face);
		virtual void enableCulling(CullType type);
		virtual void enableCulling(CullFace face);
		virtual void enableCulling();
		virtual void disableCulling();

		//---Clip-plains--------------------------
		virtual void clipPlane(const Vec4 &plain, int plainNum);
		virtual void enableClipPlane(int plainNum);
		virtual void enableClipPlane(const Vec4 &plain, int plainNum);
		virtual void disableClipPlane(int plainNum);

		//---Matrix-works-------------------------
		virtual void setMatrixMode_Projection();
		virtual void setMatrixMode_Modelview();

		virtual void pop();
		virtual void push();
		virtual void identity();

		virtual Mat4 getMatrix_MVP();
		virtual Mat4 getMatrix_Projection();
		virtual Mat4 getMatrix_Modelview();

		virtual void multMatrix(const Mat4 &matrix);
		virtual void loadMatrix(const Mat4 &matrix);

		virtual void translate(const Vec3 &pos);
		virtual void rotate(float angle, const Vec3 &axis);
		virtual void scale(const Vec3 &coef);

		//---Draw-Geom-Stream---------------------
		virtual void drawIndexedGeometry(void *indices, int indexCount);
		virtual void drawGeometry(int vertexCount);

	private:
		friend class GLTexture;
		friend class GLVBO;
		friend class GLShader;
		friend class GLDisplayList;
	};
}