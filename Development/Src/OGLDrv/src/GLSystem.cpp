#include "RenderPrivate.h"
//***************************************************************************
#include "Error.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLTexture.h"
#include "CommonDefs.h"
//***************************************************************************
#include "../../../Externals/glfw/include/GLFW/glfw3.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	unsigned int GLSystem::drawCalls = 0;
	unsigned int GLSystem::maxDrawCalls = 0;

	unsigned int GLSystem::renderStateChanges = 0;
	unsigned int GLSystem::maxRenderStateChanges = 0;

	size_t GLSystem::gpuMemoryWrite = 0;
	size_t GLSystem::maxGPUMemoryWrite = 0;

	size_t GLSystem::gpuMemoryRead = 0;
	size_t GLSystem::maxGPUMemoryRead = 0;

	/**
	*/
	GLSystem::GLSystem(CoreManager*_engine)
		:engine(_engine), cvars(engine->cvars), polygon_cull(0), polygon_front(0), debugOutputEnabled(false)
	{}

	/**
	*/
	void ManageVSync(CoreManager*engine, bool v)
	{
#ifndef DROP_EDITOR
		if (engine->mIsEditor)
			wglSwapIntervalEXT(v);
		else
#endif
			engine->iWindow->ManageVSync(v);
	}

	/**
	*/
	static void APIENTRY ReportGLError(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void *userdata)
	{
		if (type >= GL_DEBUG_TYPE_ERROR && type <= GL_DEBUG_TYPE_PERFORMANCE)
		{
			if (source == GL_DEBUG_SOURCE_API)
				LogPrintf("GL(%i): id: %i : %s", severity, id, message);
			else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER)
				LogPrintf("GLSL(%i): id: %i : %s", severity, id, message);
			else
				LogPrintf("OTHER(%i): id: %i : %s", severity, id, message);
		}
	}

	/**
	*/
	void GLSystem::_RenderSetDefaults()
		{
		//clear display
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0);

		//enable depth testing and culling
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//Texture Params
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	/**
	*/
	void GLSystem::initialise()	{
#ifndef DROP_EDITOR
		if (!createContext(GetWindow()))
		{
			Error("[GLSystem] initialise-Failed Creation OpenGL Context", true);
			return;
		}
#endif
		Log::writeHeader("-- GLSystem --");
		LogPrintf("Vendor:%s", _GetVendor());
		LogPrintf("Renderer:%s", _GetRenderer());
		LogPrintf("Version:%s", _GetVersion());
		LogPrintf("GLSL Version:%s", _GetGLSLVersion());
		/*
		*/
		defAniso = GLTexture::ANISO_X0;
		defFilter = GLTexture::LINEAR_MIPMAP_LINEAR;
		/*
		*/
		_RenderSetDefaults();
		/*
		*/
		reshape(GetWindow()->getWidth(), GetWindow()->getHeight());
		/*
		*/
		GLExtensions::initExtensions();
		/*
		*/
		ManageVSync(engine, false);
		/*
		*/
		EnableMultiSample(false);
#ifdef _ENGINE_DEBUG_
		requireExtension("GL_ARB_debug_output", false);
		EnableDebugOutput();
#endif
		/*
		*/
		{
			FullscreenQuadVertex verts[] = {
				{ { -1.0F, -3.0F } },
				{ { -1.0F, 1.0F } },
				{ { 3.0F, 1.0F } }
			};

			VertexLayout layout;
			layout.stride = sizeof(FullscreenQuadVertex);
			layout.attributes.emplace_back(2, 0, false, DataType::Float);

			_fullscreenTriangle = new MeshObject();
			_fullscreenTriangle->GetVertexBuffer().Allocate(
				verts,
				3 * sizeof(FullscreenQuadVertex),
				GLVBO::STATIC
				);
			_fullscreenTriangle->SetVertexLayout(layout);
		}
	}

	/**
	*/
	void GLSystem::DrawArrays(
		unsigned int baseVertex,
		unsigned int numVertices,
		MeshPrimitiveType type
		)
	{
		glDrawArrays(
			static_cast<GLenum>(type),
			baseVertex,
			numVertices
			);
		drawCalls++;
	}

	/**
	*/
	void GLSystem::DrawFullscreenQuad()
	{
		_fullscreenTriangle->Bind();
		DrawArrays(0, 3);
	}

	/**
	*/
	void GLSystem::FlushCommandBuffer()
	{
		glFlush();
		glFinish();
	}

	/**
	*/
	void GLSystem::WriteScreenshot(const char* path)
	{
		size_t unused = 42;
		unsigned char* output;

		unsigned int wx = mCurrentViewport.x;
		unsigned int wy = mCurrentViewport.y;
		unsigned int ww = mCurrentViewport.z;
		unsigned int wh = mCurrentViewport.w;

		output = (unsigned char*)malloc((ww - wx) * (wh - wy) * 3 * sizeof(char));

		glReadBuffer(GL_FRONT);
		glReadPixels(wx, wy, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, output);

		unsigned char tgaHeader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		unsigned char header[6];
		unsigned char bits = 0;
		int colorMode = 0;
		unsigned char tempColors = 0;
		FILE* file = fopen(path, "wb");
		if (!file) {
			free(output);
			Error(String("Cannot create screenshot file: ") + path, false);
			return;
		}
		colorMode = 3;
		bits = 24;
		header[0] = (int)(ww - wx) % 256;
		header[1] = (int)(ww - wx) / 256;
		header[2] = (int)(wh - wy) % 256;
		header[3] = (int)(wh - wy) / 256;
		header[4] = bits;
		header[5] = 0;
		unused = fwrite(tgaHeader, sizeof(tgaHeader), 1, file);
		unused = fwrite(header, sizeof(header), 1, file);
		for (size_t i = 0; i < (ww - wx) * (wh - wy) * colorMode; i += colorMode) {
			tempColors = output[i];
			output[i] = output[i + 2];
			output[i + 2] = tempColors;
		}
		unused = fwrite(output, (ww - wx) * (wh - wy) * colorMode, 1, file);
		free(output);
		fclose(file);
	}

	/**
	*/
	void GLSystem::EnableDebugOutput()
	{
#ifdef _ENGINE_DEBUG_
		if (glDebugMessageCallback){
			Debug("Enabling GL debug output");
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(ReportGLError, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
			debugOutputEnabled = true;
		}
		else
			Debug("glDebugMessageCallback not available");
#endif
	}

	/**
	*/
	void GLSystem::EnableWireframeMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	/**
	*/
	void GLSystem::DisableWireframeMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	/**
	*/
	void GLSystem::DisableDebugOutput()
	{
#ifdef _ENGINE_DEBUG_
		Debug("Disabling GL debug output");
		glDisable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(nullptr, nullptr);
		debugOutputEnabled = false;
#endif
	}

	/**
	*/
	void GLSystem::EnableMultiSample(bool v){
		if (v)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}

	/**
	*/
	void GLSystem::EnableVSync(bool _v){
		ManageVSync(engine, _v);
	}

	/**
	*/
	GLSystem::~GLSystem() {
		SAFE_DELETE(_fullscreenTriangle);
	}

	/**
	*/
	const char * GLSystem::_GetVendor() {
		return (const char *)glGetString(GL_VENDOR);
	}

	/**
	*/
	const char * GLSystem::_GetRenderer() {
		return (const char *)glGetString(GL_RENDERER);
	}

	/**
	*/
	const char * GLSystem::_GetVersion() {
		return (const char *)glGetString(GL_VERSION);
	}

	/**
	*/
	const char* GLSystem::_GetGLSLVersion() {
		return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	/**
	*/
	bool GLSystem::requireExtension(const char*name, bool _fatal) {
		if (!GLExtensions::isExtSupported(name))
		{
			if (_fatal)
				Error::showAndExit(String("GLSystem::requireExtension() error: your video card does not support ") + name);
			else
				Warning("GLSystem::requireExtension() error: your video card does not support %s", name);
			return false;
		}
		return true;
	}

	/**
	*/
	void GLSystem::reshape(int width, int height) {
		if (height == 0)
			height = 1;

		setViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		loadMatrix(Mat4::perspective(cvars->cl_fov, (float)width / (float)height, 1, 500));//TODO:FOV
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	/**
	*/
	void GLSystem::getViewport(int *viewport) {
		glGetIntegerv(GL_VIEWPORT, viewport);
	}

	/*
	*/
	void GLSystem::clearColor(const Vec3 &color) {
		glClearColor(color.x, color.y, color.z, 1.0);
	}

	/**
	*/
	void GLSystem::colorMask(bool r, bool g, bool b, bool a) {
		glColorMask(r, g, b, a);
	}

	/**
	*/
	void GLSystem::clear(GLbitfield buffers) {
		glClear(buffers);
	}

	/**
	*/
	void GLSystem::flush() {
		glFlush();
	}

	/**
	*/
	void GLSystem::setViewport(unsigned int w, unsigned int h) {
		mCurrentViewport = Vec4(0, 0, w, h);
		glViewport(0, 0, w, h);
	}

	/**
	*/
	void GLSystem::setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		mCurrentViewport = Vec4(x, y, w, h);
		glViewport(x, y, w, h);
	}

	/**
	*/
	Mat4 GLSystem::getMatrix_MVP() {
		return getMatrix_Projection() * getMatrix_Modelview();
	}

	/**
	*/
	Mat4 GLSystem::getMatrix_Projection() {
		Mat4 temp;
		glGetFloatv(GL_PROJECTION_MATRIX, temp);
		return temp;
	}

	/**
	*/
	Mat4 GLSystem::getMatrix_Modelview() {
		Mat4 temp;
		glGetFloatv(GL_MODELVIEW_MATRIX, temp);
		return temp;
	}

	/**
	*/
	void GLSystem::multMatrix(const Mat4 &matrix) {
		glMultMatrixf(matrix);//Deprecated
	}

	/**
	*/
	void GLSystem::loadMatrix(const Mat4 &matrix) {
		glLoadMatrixf(matrix);//Deprecated
	}

	/**
	*/
	void GLSystem::setMatrixMode_Projection() {
		glMatrixMode(GL_PROJECTION);//Deprecated
	}

	/**
	*/
	void GLSystem::setMatrixMode_Modelview() {
		glMatrixMode(GL_MODELVIEW);//Deprecated
	}

	/**
	*/
	void GLSystem::push() {
		glPushMatrix();//Deprecated
	}

	/**
	*/
	void GLSystem::pop() {
		glPopMatrix();//Deprecated
	}

	/**
	*/
	void GLSystem::identity() {
		glLoadIdentity();
	}

	/**
	*/
	void GLSystem::translate(const Vec3 &pos) {
		glTranslatef(pos.x, pos.y, pos.z);
	}

	/**
	*/
	void GLSystem::rotate(float angle, const Vec3 &axis) {
		glRotatef(angle, axis.x, axis.y, axis.z);
	}

	/**
	*/
	void GLSystem::scale(const Vec3 &coef) {
		glScalef(coef.x, coef.y, coef.z);
	}

	/**
	*/
	void GLSystem::setColor(const Vec3 &color) {
		glColor3fv(color);
	}

	/**
	*/
	void GLSystem::setColor(const Vec4 &color) {
		glColor4fv(color);
	}

	/**
	*/
	void GLSystem::SetPolygonFront(CullType front)
	{
		assert(front >= 0 && front < NUM_FRONT_MODES && "GLSystem::SetPolygonFront(): bad polygon front");

		static const GLuint front_modes[] = {
			GL_CCW,
			GL_CW,
		};

		if (front == CULL_FRONT) {
			if (polygon_front == CULL_CCW)
			{
				glFrontFace(front_modes[CULL_CW]);
				polygon_front = CULL_CW;
			}
			else if (polygon_front == CULL_CW)
			{
				glFrontFace(front_modes[CULL_CCW]);
				polygon_front = CULL_CCW;
			}
		}
		else if (polygon_front != front)
		{
			glFrontFace(front_modes[front]);
			polygon_front = front;
		}
	}

	/**
	*/
	void GLSystem::SetPolygonCull(CullFace cull)
	{
		assert(cull >= 0 && cull < NUM_CULL_MODES && "GLSystem::SetPolygonCull(): bad polygon cull");

		static const GLuint cull_modes[] = {
			0,
			GL_BACK,
			GL_FRONT,
		};

		if (polygon_cull != cull)
		{
			if (cull == CULL_NONE)
			{
				glDisable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else
			{
				glCullFace(cull_modes[cull]);
				if (polygon_cull == CULL_NONE) glEnable(GL_CULL_FACE);
			}
			polygon_cull = cull;
		}
	}

	/**
	*/
	void GLSystem::enableCulling(CullType type) {
		glEnable(GL_CULL_FACE);
		SetPolygonFront(type);
	}

	/**
	*/
	void GLSystem::enableCulling(CullFace face) {
		glEnable(GL_CULL_FACE);
		SetPolygonCull(face);
	}

	/**
	*/
	void GLSystem::enableCulling() {
		glEnable(GL_CULL_FACE);
	}

	/**
	*/
	void GLSystem::disableCulling() {
		glDisable(GL_CULL_FACE);
		polygon_cull = CULL_NONE;
	}

	/**
	*/
	void GLSystem::alphaTestFunc(CompareType type, float alphaRef) {
		static const GLuint modes[] = {
			/*NEVER*/0x0209, /*LESS*/ 0x0201,/*EQUAL*/ 0x0202,
			/*LEQUAL*/ 0x0203, /*GREATER*/ 0x0204,/*NOTEQUAL*/ 0x0205,
			/*GEQUAL*/ 0x0206, /*ALWAYS*/ 0x0207
		};
		glAlphaFunc(modes[type], alphaRef);
	}

	/**
	*/
	void GLSystem::enableAlphaTest(CompareType type, float alphaRef) {
		glEnable(GL_ALPHA_TEST);
		static const GLuint modes[] = {
			/*NEVER*/0x0209, /*LESS*/ 0x0201,/*EQUAL*/ 0x0202,
			/*LEQUAL*/ 0x0203, /*GREATER*/ 0x0204,/*NOTEQUAL*/ 0x0205,
			/*GEQUAL*/ 0x0206, /*ALWAYS*/ 0x0207
		};
		glAlphaFunc(modes[type], alphaRef);
	}

	/**
	*/
	void GLSystem::enableAlphaTest() {
		glEnable(GL_ALPHA_TEST);
	}

	/**
	*/
	void GLSystem::disableAlphaTest() {
		glDisable(GL_ALPHA_TEST);
	}

	/**
	*/
	void GLSystem::depthFunc(CompareType type) {
		static const GLuint modes[] = {
			/*NEVER*/0x0209, /*LESS*/ 0x0201,/*EQUAL*/ 0x0202,
			/*LEQUAL*/ 0x0203, /*GREATER*/ 0x0204,/*NOTEQUAL*/ 0x0205,
			/*GEQUAL*/ 0x0206, /*ALWAYS*/ 0x0207
		};
		glDepthFunc(modes[type]);
	}

	/**
	*/
	void GLSystem::enableDepth(CompareType type) {
		glEnable(GL_DEPTH_TEST);
		static const GLuint modes[] = {
			/*NEVER*/0x0209, /*LESS*/ 0x0201,/*EQUAL*/ 0x0202,
			/*LEQUAL*/ 0x0203, /*GREATER*/ 0x0204,/*NOTEQUAL*/ 0x0205,
			/*GEQUAL*/ 0x0206, /*ALWAYS*/ 0x0207
		};
		glDepthFunc(modes[type]);
	}

	/**
	*/
	void GLSystem::enableDepth() {
		glEnable(GL_DEPTH_TEST);
	}

	/**
	*/
	void GLSystem::disableDepth() {
		glDisable(GL_DEPTH_TEST);
	}

	/**
	*/
	void GLSystem::depthMask(bool mask) {
		glDepthMask(mask);
	}

	/**
	*/
	void GLSystem::scissorRect(int x, int y, int z, int w) {
		glScissor(x, y, z, w);
	}

	/**
	*/
	void GLSystem::enableScissor(int x, int y, int z, int w) {
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, z, w);
	}

	/**
	*/
	void GLSystem::enableScissor() {
		glEnable(GL_SCISSOR_TEST);
	}

	/**
	*/
	void GLSystem::disableScissor() {
		glDisable(GL_SCISSOR_TEST);
	}

	/**
	*/
	void GLSystem::polygonOffsetFill(float a, float b) {
		glPolygonOffset(a, b);
	}

	/***
	*/
	void GLSystem::enablePolygonOffsetFill(float a, float b) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(a, b);
	}

	/**
	*/
	void GLSystem::enablePolygonOffsetFill() {
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	/**
	*/
	void GLSystem::disablePolygonOffsetFill() {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	/**
	*/
	void GLSystem::enableBlending() {
		glEnable(GL_BLEND);
	}

	/**
	*/
	void GLSystem::blendFunc(BlendParam src, BlendParam dst) {
		static const GLuint modes[] = {
			1, 0, 0x0300, 0x0306, 0x0302, 0x0304,
			0x0301, 0x0307, 0x0303, 0x0305
		};
		glBlendFunc(modes[src], modes[dst]);
	}

	/**
	*/
	void GLSystem::enableBlending(BlendParam src, BlendParam dst)
	{
		glEnable(GL_BLEND);

		static const GLuint modes[] = {
			1, 0, 0x0300, 0x0306, 0x0302, 0x0304,
			0x0301, 0x0307, 0x0303, 0x0305
		};
		glBlendFunc(modes[src], modes[dst]);
	}

	/**
	*/
	void GLSystem::disableBlending() {
		glDisable(GL_BLEND);
	}

	/**
	*/
	//---------------------------------------------------------------------------
	void GLSystem::clipPlane(const Vec4 &plain, int plainNum) {
		double eq[4] = { (double)plain[0], (double)plain[1], (double)plain[2], (double)plain[3] };

		glClipPlane(GL_CLIP_PLANE0 + plainNum, eq);
	}

	/**
	*/
	void GLSystem::enableClipPlane(int plainNum) {
		glEnable(GL_CLIP_PLANE0 + plainNum);
	}

	/**
	*/
	void GLSystem::enableClipPlane(const Vec4 &plain, int plainNum) {
		double eq[4] = { (double)plain[0], (double)plain[1], (double)plain[2], (double)plain[3] };

		glClipPlane(GL_CLIP_PLANE0 + plainNum, eq);
		glEnable(GL_CLIP_PLANE0 + plainNum);
	}

	/**
	*/
	void GLSystem::disableClipPlane(int plainNum) {
		glDisable(GL_CLIP_PLANE0 + plainNum);
	}

	/**
	*/
	void GLSystem::enable2d(bool normalized) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		if (normalized)
			loadMatrix(Mat4::ortho(0, 1, 1, 0, 0, 1));
		else
			loadMatrix(Mat4::ortho(0, GetWindow()->getWidth(), GetWindow()->getHeight(), 0, 0, 1));


		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	/**
	*/
	void GLSystem::enable3d() {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	/**
	*/
	void GLSystem::drawRect(float x0, float y0, float x3, float y3, float tx0, float ty0, float tx3, float ty3) {
		glBegin(GL_QUADS);
		glTexCoord2f(tx0, ty0);
		glVertex3f(x0, y0, 0);

		glTexCoord2f(tx0, ty3);
		glVertex3f(x0, y3, 0);

		glTexCoord2f(tx3, ty3);
		glVertex3f(x3, y3, 0);

		glTexCoord2f(tx3, ty0);
		glVertex3f(x3, y0, 0);
		glEnd();
	}

	/**
	*/
	void GLSystem::DrawElements(void *indices, int indexCount) {
		//������� ����������� ���
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
		drawCalls++;
	}

	/**
	*/
	void GLSystem::DrawArrays(int vertexCount) {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	/**
	*/
	bool GLSystem::createContext(I_Window* _window)
	{
#ifndef DROP_EDITOR
		if (engine->mIsEditor)
		{
			Debug("[Render]GLSystem::_createOldContext");
			static PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,											// Version Number
				PFD_DRAW_TO_WINDOW |						// Format Must Support Window
				PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,							// Must Support Double Buffering
				PFD_TYPE_RGBA,								// Request An RGBA Format
				_window->bpp,								// Select Our Color Depth
				0, 0, 0, 0, 0, 0,							// Color Bits Ignored
				0,											// No Alpha Buffer
				0,											// Shift Bit Ignored
				0,											// No Accumulation Buffer
				0, 0, 0, 0,									// Accumulation Bits Ignored
				_window->zdepth,							// Z-Buffer (Depth Buffer)  
				0,											// Stencil Buffer
				0,											// No Auxiliary Buffer
				PFD_MAIN_PLANE,								// Main Drawing Layer
				0,											// Reserved
				0, 0, 0										// Layer Masks Ignored
			};

			if (!(_window->hDC = GetDC((HWND)_window->hWnd)))	{
				Error("WindowSystem::initialise() error: can't create a GL device context", true);
				return false;
			}

			if (!(_window->pixelFormat = ChoosePixelFormat((HDC)_window->hDC, &pfd))) {
				Error("WindowSystem::initialise() error: can't find a suitable pixel format", true);
				return false;
			}

			if (!(SetPixelFormat((HDC)_window->hDC, _window->pixelFormat, &pfd)))	{
				Error("WindowSystem::initialise() error: can't set the pixel format", true);
				return false;
			}

			if (!(_window->hRC = wglCreateContext((HDC)_window->hDC)))	{
				Error("WindowSystem::initialise() error: can't create a GL rendering context", true);
				return false;
			}

			if (!wglMakeCurrent((HDC)_window->hDC, (HGLRC)_window->hRC)) {
				Error("WindowSystem::initialise() error: can't activate the GL rendering context", true);
				return false;
			}
			return true;
		}

else
#endif
		{
			Debug("Implemented in {WindowSystemGLFW::initialise} because crossplatform");
			return true;
		}

		return false;
	}

	/**
	*/
	void GLSystem::swapBuffers() {
#ifndef DROP_EDITOR
		if (engine->mIsEditor)
			SwapBuffers((HDC)engine->iWindow->hDC);
		else
#endif
			engine->iWindow->swapBuffers();
	}
}