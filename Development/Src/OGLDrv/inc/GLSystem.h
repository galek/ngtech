#pragma once 

//***************************************************************************
#include "MathLib.h"
#include "GLExtensions.h"
#include "GLTexture.h"
#include "GLVBO.h"
#include "GLShader.h"
#include "GLDisplayList.h"
#include "GLFBO.h"
#include "GLOcclusionQuery.h"
#include "RenderDLL.h"
#include <vector>
#include "meshobject.hh"
//***************************************************************************

namespace NGTech {
	class Engine;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main video system. Created one time
	//---------------------------------------------------------------------------
	class RENDER_API GLSystem :public I_Render {
		friend class GLVBO;
	public:
		virtual void EnableMultiSample(bool);
		virtual void EnableVSync(bool);
		void EnableDebugOutput();
		void DisableDebugOutput();
		virtual void EnableWireframeMode();
		virtual void DisableWireframeMode();
	public:
		GLSystem(CoreManager*);
		virtual ~GLSystem();

		virtual void initialise();

		virtual void reshape(int width, int height);
		virtual void getViewport(int *viewport);
		virtual void clearColor(const Vec3 &color);
		virtual void colorMask(bool r, bool g, bool b, bool a);
		virtual void clear(GLbitfield buffers);
		virtual void flush();
		virtual void setViewport(unsigned int w, unsigned int h);
		virtual void setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

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
		virtual void SetPolygonFront(CullType type);
		virtual void SetPolygonCull(CullFace face);
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
		virtual void DrawElements(void *indices, int indexCount);
		virtual void DrawArrays(int vertexCount);

		bool requireExtension(const char*name, bool _fatal = false);

		void WriteScreenshot(const char* path);

		// drawing
		void DrawArrays(unsigned int baseVertex, unsigned int numVertices, MeshPrimitiveType type = MeshPrimitiveType::Triangles);

		// fullscreen quad
		void DrawFullscreenQuad();

		void FlushCommandBuffer();

		// profiling
		static void RecordMemoryWrite(size_t numBytes);
		static void RecordMemoryRead(size_t numBytes);

		static unsigned int GetDrawCalls();
		static unsigned int GetMaxDrawCalls();

		static unsigned int GetRenderStateChanges();
		static unsigned int GetMaxRenderStateChanges();

		static float GetGPUMemoryWrite();
		static float GetMaxGPUMemoryWrite();

		static float GetGPUMemoryRead();
		static float GetMaxGPUMemoryRead();

		ENGINE_INLINE bool IsDebugOutputEnabled() const	{ return debugOutputEnabled; }
	private:
		virtual bool createContext(I_Window*);
		virtual void swapBuffers();
		/*
		OpenGL info
		*/
		const char * _GetVendor();
		const char * _GetRenderer();
		const char * _GetVersion();
		const char* _GetGLSLVersion();
		/*
		Set default options
		*/
		void _RenderSetDefaults();
	private:
		//FABRICS
		virtual GLDisplayList* GetDL();
		virtual GLOcclusionQuery* GetOQ();


		virtual GLTexture* TextureCreate2D(const String &path);
		virtual GLTexture* TextureCreateCube(const String &path);
		virtual GLTexture *TextureCreate2D(int width, int height, int format);
		virtual GLTexture *TextureCreate3D(int width, int height, int depth, int format);
		virtual GLTexture *TextureCreateCube(int width, int height, int format);
		virtual GLTexture *TextureCreate2D(I_ILImage *image);
		virtual GLTexture *TextureCreate3D(I_ILImage *image);
		virtual GLTexture *TextureCreateCube(I_ILImage **image);

		virtual GLShader *ShaderCreate(const String &path, const String &defines = "");
		virtual GLShader *ShaderCreateVSandFS(const String &pathFS, const String &pathVS, const String &defines = "");
		virtual GLVBO *CreateIBO(void *data, int numElements, int elemSize, int dataType);
		virtual GLVBO *CreateVBO(void *data, int numElements, int elemSize, int dataType, int drawType);
		virtual GLFBO *CreateFBO(int x, int y);


		virtual ILImage* CreateImage2D(const String &path);
		virtual ILImage* CreateImageEmpty2D(int width, int height, int format);
		virtual ILImage* CreateImageNoise2D(int width, int height, int format);
		virtual ILImage* CreateImageEmpty3D(int width, int height, int depth, int format);
		virtual ILImage* CreateImageNoise3D(int width, int height, int depth, int format);
		virtual void*    GetGUIImageLoader();
	private:
		friend class GLTexture;
		friend class GLVBO;
		friend class GLShader;
		friend class GLDisplayList;
	private:
		Vec4 mCurrentViewport;
		unsigned int polygon_cull;
		unsigned int polygon_front;
		MeshObject *_fullscreenTriangle;
		bool debugOutputEnabled;
	private:
		static unsigned int drawCalls;
		static unsigned int maxDrawCalls;

		static unsigned int renderStateChanges;
		static unsigned int maxRenderStateChanges;

		static size_t gpuMemoryWrite;
		static size_t maxGPUMemoryWrite;

		static size_t gpuMemoryRead;
		static size_t maxGPUMemoryRead;
	private:
		CoreManager* engine;
		CVARManager* cvars;
	};

	ENGINE_INLINE void GLSystem::RecordMemoryWrite(size_t numBytes)
	{
		gpuMemoryWrite += numBytes;
	}

	ENGINE_INLINE void GLSystem::RecordMemoryRead(size_t numBytes)
	{
		gpuMemoryRead += numBytes;
	}

	ENGINE_INLINE unsigned int GLSystem::GetDrawCalls()
	{
		return drawCalls;
	}

	ENGINE_INLINE unsigned int GLSystem::GetMaxDrawCalls()
	{
		return maxDrawCalls;
	}

	ENGINE_INLINE unsigned int GLSystem::GetRenderStateChanges()
	{
		return renderStateChanges;
	}

	ENGINE_INLINE unsigned int GLSystem::GetMaxRenderStateChanges()
	{
		return maxRenderStateChanges;
	}

	ENGINE_INLINE float GLSystem::GetGPUMemoryRead()
	{
		return gpuMemoryRead;
	}

	ENGINE_INLINE float GLSystem::GetMaxGPUMemoryRead()
	{
		return maxGPUMemoryRead;
	}

	ENGINE_INLINE float GLSystem::GetGPUMemoryWrite()
	{
		return gpuMemoryWrite;
	}

	ENGINE_INLINE float GLSystem::GetMaxGPUMemoryWrite()
	{
		return maxGPUMemoryWrite;
	}

	struct FullscreenQuadVertex
	{
		float pos[2];
	};
	static_assert(sizeof(FullscreenQuadVertex) == 8, "Invalid structure size!");
}