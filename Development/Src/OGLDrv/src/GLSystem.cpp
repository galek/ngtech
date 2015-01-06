#include "RenderPrivate.h"
//***************************************************************************
#include "Engine.h"
#include "GLSystem.h"
#include "Log.h"
#include "WindowSystem.h"
#include "GLTexture.h"
#include "Error.h"
#include "glew/wglew.h"
//***************************************************************************
#include "../../Platform/inc/glfw/glfw3.h"
//***************************************************************************

namespace NGTech {

	/**
	*/
	GLDisplayList* GLSystem::GetDL(){ return new GLDisplayList(); }

	/**
	*/
	GLOcclusionQuery* GLSystem::GetOQ() { return new GLOcclusionQuery(); }

	/**
	*/
	GLTexture* GLSystem::TextureCreate2D(const String &path){ return GLTexture::create2d(path); }

	/**
	*/
	GLTexture* GLSystem::TextureCreateCube(const String &path){ return GLTexture::createCube(path); }

	/**
	*/
	GLTexture *GLSystem::TextureCreate2D(I_ILImage *image) { return GLTexture::create2d(image); }

	/**
	*/
	GLTexture *GLSystem::TextureCreate3D(I_ILImage *image) { return GLTexture::create3d(image); }

	/**
	*/
	GLTexture *GLSystem::TextureCreateCube(I_ILImage **image) { return GLTexture::createCube(image); }

	/**
	*/
	GLTexture* GLSystem::TextureCreate2D(int width, int height, int format) { return GLTexture::create2d(width, height, (I_Texture::Format)format); }

	/**
	*/
	GLTexture* GLSystem::TextureCreate3D(int width, int height, int depth, int format) { return GLTexture::create3d(width, height, depth, (I_Texture::Format)format); }

	/**
	*/
	GLTexture* GLSystem::TextureCreateCube(int width, int height, int format) { return GLTexture::createCube(width, height, (I_Texture::Format)format); }

	/**
	*/
	GLShader  *GLSystem::ShaderCreate(const String &path, const String &defines){ return GLShader::create(path, defines); }

	/**
	*/
	GLShader  *GLSystem::ShaderCreateVSandFS(const String &pathVS, const String &pathFS, const String &defines){ return GLShader::createFromPath(pathVS, pathFS, defines); }

	/**
	*/
	GLFBO*GLSystem::CreateFBO(int x, int y){ return GLFBO::create(x, y); }

	/**
	*/
	GLVBO *GLSystem::CreateIBO(void *data, int numElements, int elemSize, int dataType){ return GLVBO::createIBO(data, numElements, elemSize, (I_VBManager::DataType)dataType); }

	/**
	*/
	GLVBO *GLSystem::CreateVBO(void *data, int numElements, int elemSize, int dataType, int drawType){ return GLVBO::createVBO(data, numElements, elemSize, (I_VBManager::DataType)dataType, (I_VBManager::TypeDraw)drawType); }

	/**
	*/
	ILImage* GLSystem::CreateImage2D(const String &path){ return ILImage::create2d(path); }

	/**
	*/
	ILImage* GLSystem::CreateImageEmpty2D(int width, int height, int format){ return ILImage::createEmpty2d(width, height, format); }

	/**
	*/
	ILImage* GLSystem::CreateImageNoise2D(int width, int height, int format){ return ILImage::createNoise2d(width, height, format); }

	/**
	*/
	ILImage* GLSystem::CreateImageEmpty3D(int width, int height, int depth, int format){ return ILImage::createEmpty3d(width, height, depth, format); }

	/**
	*/
	ILImage* GLSystem::CreateImageNoise3D(int width, int height, int depth, int format){ return ILImage::createNoise3d(width, height, depth, format); }

	/**
	*/
	GLSystem::GLSystem(CoreManager*_engine)
		:engine(_engine), polygon_cull(0), polygon_front(0)
	{}

	/**
	*/
	void DisableVSync(CoreManager*engine)
	{
#ifndef DROP_EDITOR
		if (engine->mIsEditor)
			wglSwapIntervalEXT(0);
		else
#endif
			engine->iWindow->DisableVSync(0);
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
		void *userdata)
	{
		if (type >= GL_DEBUG_TYPE_ERROR && type <= GL_DEBUG_TYPE_PERFORMANCE)
		{
			if (source == GL_DEBUG_SOURCE_API)
				LogPrintf("GL(", severity, "): ");
			else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER)
				LogPrintf("GLSL(", severity, "): ");
			else
				LogPrintf("OTHER(", severity, "): ");

			LogPrintf("", id, ": ", message);
		}
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
		Log::write("Vendor:         " + getVendor());
		Log::write("Renderer:       " + getRenderer());
		Log::write("Version:        " + getVersion());
		Log::write("Texture units:  " + StringHelper::fromInt(getNumTexUnits()));
		Log::write("Max anisotropy: " + StringHelper::fromInt(getMaxAniso()));

		defAniso = GLTexture::ANISO_X0;
		defFilter = GLTexture::LINEAR_MIPMAP_LINEAR;


		//enable depth testing and culling
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		//Texture Params
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		reshape(GetWindow()->getWidth(), GetWindow()->getHeight());
		GLExtensions::initExtensions();
		DisableVSync(engine);

#ifdef _ENGINE_DEBUG_
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
		glDebugMessageCallback(ReportGLError, 0);
#endif
	}

	/**
	*/
	GLSystem::~GLSystem() {
	}

	/**
	*/
	String GLSystem::getVendor() {
		return (char *)glGetString(GL_VENDOR);
	}

	/**
	*/
	String GLSystem::getRenderer() {
		return (char *)glGetString(GL_RENDERER);
	}

	/**
	*/
	String GLSystem::getVersion() {
		return (char *)glGetString(GL_VERSION);
	}

	/**
	*/
	int GLSystem::getNumTexUnits() {
		int nTexUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (int *)&nTexUnits);
		return nTexUnits;
	}

	/**
	*/
	int GLSystem::getMaxAniso() {
		int maxAniso;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		return maxAniso;
	}

	/**
	*/
	bool GLSystem::requireExtension(const String &name, bool _fatal) {
		if (!GLExtensions::isExtSupported(name))
		{
			if (_fatal)
				Error::showAndExit("GLSystem::requireExtension() error: your video card does not support " + name);
			else
				Warning("GLSystem::requireExtension() error: your video card does not support %s", name.c_str());
			return false;
		}
		return true;
	}

	/**
	*/
	void GLSystem::reshape(int width, int height) {
		if (height == 0)
			height = 1;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		loadMatrix(Mat4::perspective(60, (float)width / (float)height, 1, 500));

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
	void GLSystem::viewport(int x, int y) {
		glViewport(0, 0, x, y);
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
		glMultMatrixf(matrix);
	}

	/**
	*/
	void GLSystem::loadMatrix(const Mat4 &matrix) {
		glLoadMatrixf(matrix);//Deprecated
	}

	/**
	*/
	void GLSystem::setMatrixMode_Projection() {
		glMatrixMode(GL_PROJECTION);
	}

	/**
	*/
	void GLSystem::setMatrixMode_Modelview() {
		glMatrixMode(GL_MODELVIEW);
	}

	/**
	*/
	void GLSystem::push() {
		glPushMatrix();
	}

	/**
	*/
	void GLSystem::pop() {
		glPopMatrix();
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
		glAlphaFunc(type, alphaRef);
	}

	/**
	*/
	void GLSystem::enableAlphaTest(CompareType type, float alphaRef) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(type, alphaRef);
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
		glDepthFunc(type);
	}

	/**
	*/
	void GLSystem::enableDepth(CompareType type) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(type);
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
		glBlendFunc(src, dst);
	}

	/**
	*/
	void GLSystem::enableBlending(BlendParam src, BlendParam dst) {
		glEnable(GL_BLEND);
		glBlendFunc(src, dst);
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
	void GLSystem::drawIndexedGeometry(void *indices, int indexCount) {
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
	}

	/**
	*/
	void GLSystem::drawGeometry(int vertexCount) {
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

#endif
		else
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