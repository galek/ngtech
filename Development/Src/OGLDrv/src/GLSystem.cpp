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
	GLFBO*GLSystem::CreateFBO(int x, int y){ return GLFBO::create(x, y); }

	/**
	*/
	GLVBO *GLSystem::CreateIBO(void *data, int numElements, int elemSize, int dataType){ return GLVBO::createIBO(data, numElements, elemSize, (I_VBManager::DataType)dataType); }

	/**
	*/
	GLVBO *GLSystem::CreateVBO(void *data, int numElements, int elemSize, int dataType){ return GLVBO::createVBO(data, numElements, elemSize, (I_VBManager::DataType)dataType); }

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
	GLSystem::GLSystem() {}

	/**
	*/
	void GLSystem::initialise()	{
		if (!createContext(GetWindow()))
		{
			Error("[GLSystem] initialise-Failed Creation OpenGL Context", true);
			return;
		}
		Log::writeHeader("-- GLSystem --");
		Log::write("Vendor:         " + getVendor());
		Log::write("Renderer:       " + getRenderer());
		Log::write("Version:        " + getVersion());
		Log::write("Extensions:     " + getExtensions());
		Log::write("Texture units:  " + StringHelper::fromInt(getNumTexUnits()));
		Log::write("Max anisotropy: " + StringHelper::fromInt(getMaxAniso()));
		defAniso = GLTexture::ANISO_X0;
		defFilter = GLTexture::LINEAR_MIPMAP_LINEAR;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		reshape(GetWindow()->getWidth(), GetWindow()->getHeight());
		GLExtensions::initExtensions();
		Debug("[Init] Checking Render Extensions");

		requireExtension("GL_ARB_vertex_shader", true);
		requireExtension("GL_ARB_fragment_shader", true);
		requireExtension("GL_ARB_shader_objects", true);
		requireExtension("GL_ARB_shading_language_100", true);
		requireExtension("GL_ARB_vertex_buffer_object", true);
		requireExtension("GL_EXT_framebuffer_object", true);
		requireExtension("GL_ARB_occlusion_query", true);
		requireExtension("GL_EXT_texture_filter_anisotropic", true);

		//OpenGL3 and 4
		requireExtension("GL_ARB_occlusion_query2");
		requireExtension("GL_ARB_compatibility");
		requireExtension("GL_ARB_shading_language_420pack");
		requireExtension("GL_ARB_geometry_shader4");
		requireExtension("GL_ARB_shading_language_packing");
		requireExtension("GL_ARB_ES2_compatibility");


		//4.X
		requireExtension("GL_ARB_compute_shader");
		requireExtension("GL_ARB_gpu_shader5");
		requireExtension("GL_ARB_tessellation_shader");
		requireExtension("GL_ARB_shader_subroutine");
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
	String GLSystem::getExtensions() {
		return (char *)glGetString(GL_EXTENSIONS);
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
		glLoadMatrixf(matrix);
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
	void GLSystem::cullFunc(CullType type) {
		glFrontFace(type);
	}

	/**
	*/
	void GLSystem::cullFace(CullFace face) {
		glFrontFace(face);
	}

	/**
	*/
	void GLSystem::enableCulling(CullType type) {
		glEnable(GL_CULL_FACE);
		glFrontFace(type);
	}

	/**
	*/
	void GLSystem::enableCulling(CullFace face) {
		glEnable(GL_CULL_FACE);
		glFrontFace(face);
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
	bool GLSystem::_createOldContext(IWindow* _window)
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

		if (!(_window->hDC = GetDC(_window->hWnd)))	{
			Error("WindowSystem::initialise() error: can't create a GL device context", true);
			return false;
		}

		if (!(_window->pixelFormat = ChoosePixelFormat(_window->hDC, &pfd))) {
			Error("WindowSystem::initialise() error: can't find a suitable pixel format", true);
			return false;
		}

		if (!(SetPixelFormat(_window->hDC, _window->pixelFormat, &pfd)))	{
			Error("WindowSystem::initialise() error: can't set the pixel format", true);
			return false;
		}

		if (!(_window->hRC = wglCreateContext(_window->hDC)))	{
			Error("WindowSystem::initialise() error: can't create a GL rendering context", true);
			return false;
		}

		if (!wglMakeCurrent(_window->hDC, _window->hRC)) {
			Error("WindowSystem::initialise() error: can't activate the GL rendering context", true);
			return false;
		}
		return true;
	}

	/**
	*/
#ifdef _WIN32
	/** WGL function pointers for OpenGL 3.x/4.x context creation */
	static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
#endif

	/**
	*/
	bool GLSystem::_checkContextSuppoort(IWindow* _window)
	{
		Debug("[Render]GLSystem::_checkContextSuppoort");

		// получим адрес функции создания расширенного контекста OpenGL
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		// временный контекст OpenGL нам больше не нужен, удаляем его
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_window->hRC);

		// если драйвер видеокарты не предоставил нам адрес этой функции
		if (!wglCreateContextAttribsARB)
		{
			Warning("wglCreateContextAttribsARB fail (%d)\n", GetLastError());
			return false;
		}
		return true;
	}

	/**
	*/
	bool GLSystem::_createNewContext(IWindow* _window)
	{
		Debug("[Render]GLSystem::_createNewContext");
		int pixfmt[8];
		unsigned int numpf;
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

		const int piAttribIList[] = {
			WGL_DRAW_TO_WINDOW_ARB, TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_SUPPORT_OPENGL_ARB, TRUE,
			WGL_DOUBLE_BUFFER_ARB, TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 24,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0, 0
		};
		// укажем атрибуты для создания расширенного контекста OpenGL 3.3
		// атрибуты установлены согласно спецификации расширения:

		// WGL_CONTEXT_MAJOR_VERSION_ARB - старшая цифра необходимой версии

		// WGL_CONTEXT_MINOR_VERSION_ARB - младшая цифра необходимой версии

		// WGL_CONTEXT_FLAGS_ARB - флаги контекста, для нас это контекст с поддержкой
		//    нового функционала WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB

		// WGL_CONTEXT_PROFILE_MASK_ARB - профиль создаваемого контекста, выбираем
		//    WGL_CONTEXT_CORE_PROFILE_BIT_ARB, все устройства с OpenGL 3.2 и старше
		//    должны поддерживать профиль CORE, этот профиль позволяет получить доступ
		//    к новому функционалу и говорит об отказе от устаревшего функционала

		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//Nick:TODO:WGL_CONTEXT_CORE_PROFILE_BIT_ARB support
			0, 0
		};

		// если драйвер видеокарты не предоставил нам адрес этой функции
		if (!wglCreateContextAttribsARB)
		{
			Warning("wglCreateContextAttribsARB fail (%d)\n", GetLastError());
			return false;
		}

		wglChoosePixelFormatARB(_window->hDC, piAttribIList, NULL, 8, pixfmt, &numpf);

		SetPixelFormat(_window->hDC, pixfmt[0], &pfd);
		// пробуем создать контекст с поддержкой OpenGL 3.3
		_window->hRC = wglCreateContextAttribsARB(_window->hDC, 0, attribs);

		// если создать контекст не получилось или он не устанавливается для нашего окна
		if (!_window->hRC || !wglMakeCurrent(_window->hDC, _window->hRC))
		{
			Warning("Creating render context fail (%d)\n", GetLastError());
			return false;
		}
		Debug("[RENDER] OpenGL 3 is available");
		return true;
	}

	/**
	*/
	bool GLSystem::createContext(IWindow* _window)
	{
		//Create temporary context
		bool status = _createOldContext(_window);
		if (!status)
			return false;
		//Checking OGL3 Extension
		status = 0;//_checkContextSuppoort(_window);
		if (status)
			LogPrintf("[Render]is new OGL %i", status);

		if (status)
		{
			if (!_createNewContext(_window))
				return _createOldContext(_window);
			return true;
		}

		return _createOldContext(_window);
	}

	/**
	*/
	void GLSystem::swapBuffers() {
		SwapBuffers(GetWindow()->hDC);
	}
}