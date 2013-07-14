/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/
#include "EnginePrivate.h"
//***************************************************************************
#include "Engine.h"
#include "GLSystem.h"
#include "Log.h"
#include "WindowSystem.h"
#include "Error.h"
//***************************************************************************

namespace NGEngine {


	//---------------------------------------------------------------------------
	//Desc:    creates new GLSystem
	//Params:  -
	//Returns: pointer to new GLSystem
	//---------------------------------------------------------------------------
	GLSystem::GLSystem() {}

	void GLSystem::initialise()	{
		Log::write("GLSystem::create()");

		Log::write("{");
		Log::write("Vendor:         " + getVendor()); 
		Log::write("Renderer:       " + getRenderer());
		Log::write("Version:        " + getVersion());
		Log::write("Extensions:     " + getExtensions()); 
		Log::write("Texture units:  " + StringHelper::fromInt(getNumTexUnits()));
		Log::write("Max anisotropy: " + StringHelper::fromInt(getMaxAniso()));
		defAniso  = GLTexture::ANISO_X0;
		defFilter = GLTexture::LINEAR_MIPMAP_LINEAR;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);

		glPixelStorei(GL_PACK_ALIGNMENT,   1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		reshape(engine.iWindow->getWidth(), engine.iWindow->getHeight());
		GLExtensions::initExtensions();

		Log::write("}\n");
	}

	//---------------------------------------------------------------------------
	//Desc:    GLSystem destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLSystem::~GLSystem() {
	}

	//---------------------------------------------------------------------------
	//Desc:    gets video card vendor
	//Params:  -
	//Returns: vendor name
	//---------------------------------------------------------------------------
	String GLSystem::getVendor() { 
		return (char *)glGetString(GL_VENDOR); 
	}

	//---------------------------------------------------------------------------
	//Desc:    gets video card renderer
	//Params:  -
	//Returns: renderer name
	//---------------------------------------------------------------------------
	String GLSystem::getRenderer() { 
		return (char *)glGetString(GL_RENDERER); 
	}

	//---------------------------------------------------------------------------
	//Desc:    gets OpenGL version
	//Params:  -
	//Returns: version number
	//---------------------------------------------------------------------------
	String GLSystem::getVersion() { 
		return (char *)glGetString(GL_VERSION); 
	}

	//---------------------------------------------------------------------------
	//Desc:    gets OpenGL extensions
	//Params:  -
	//Returns: extensions string
	//---------------------------------------------------------------------------
	String GLSystem::getExtensions() { 
		return (char *)glGetString(GL_EXTENSIONS); 
	} 

	//---------------------------------------------------------------------------
	//Desc:    gets number of video card texture units
	//Params:  -
	//Returns: tex units number
	//---------------------------------------------------------------------------
	int GLSystem::getNumTexUnits() {
		int nTexUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, (int *)&nTexUnits);
		//glGetIntegerv(GL_MAX_TEXTURE_UNITS, (int *)&nTexUnits);
		return nTexUnits;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets video card max level of anisotropy
	//Params:  -
	//Returns: max aniso
	//---------------------------------------------------------------------------
	int GLSystem::getMaxAniso() {
		int maxAniso;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		return maxAniso;
	}

	//---------------------------------------------------------------------------
	//Desc:    checks if extension is supported. if not - exit
	//Params:  name - extension name
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::requireExtension(const String &name) {
		if(!GLExtensions::isExtSupported(name)) {
			Error::showAndExit("GLSystem::requireExtension() error: your video card does not support " + name);
		}
	}




	//---------------------------------------------------------------------------
	//Desc:    reshapes the screen
	//Params:  width, height - new screen size
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::reshape(int width, int height) {
		if(height == 0) 
			height = 1;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		loadMatrix(Mat4::perspective(60, (float)width/(float)height, 1, 500));

		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
	}

	//---------------------------------------------------------------------------
	//Desc:    gets the viewport coords
	//Params:  viewport - pointer to array of at least 4 ints
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::getViewport(int *viewport) {
		glGetIntegerv(GL_VIEWPORT,viewport);
	}

	//---------------------------------------------------------------------------
	//Desc:    clears color of the screen
	//Params:  color - new screen color
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::clearColor(const Vec3 &color) {
		glClearColor(color.x, color.y, color.z, 1.0);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets color buffer mask
	//Params:  r, g, b, a - color masks
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::colorMask(bool r, bool g, bool b, bool a) {
		glColorMask(r, g, b, a);
	}

	//---------------------------------------------------------------------------
	//Desc:    clears listed buffers
	//Params:  buffers - list of buffers to clear
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::clear(GLbitfield buffers) {
		glClear(buffers);
	}

	//---------------------------------------------------------------------------
	//Desc:    flushes the OpenGL pipeline
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::flush() {
		glFlush();
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the viewport size
	//Params:  x, y - new viewport size
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::viewport(int x, int y) {
		glViewport(0, 0, x, y);
	}




	//---------------------------------------------------------------------------
	//Desc:    gets the current state matrix 
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Mat4 GLSystem::getMatrix_MVP() {
		return getMatrix_Projection() * getMatrix_Modelview();
	}

	//---------------------------------------------------------------------------
	//Desc:    gets the current state matrix 
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Mat4 GLSystem::getMatrix_Projection() {
		Mat4 temp;
		glGetFloatv(GL_PROJECTION_MATRIX, temp);
		return temp;
	}

	//---------------------------------------------------------------------------
	//Desc:    gets the current state matrix 
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	Mat4 GLSystem::getMatrix_Modelview() {
		Mat4 temp;
		glGetFloatv(GL_MODELVIEW_MATRIX, temp);
		return temp;
	}

	//---------------------------------------------------------------------------
	//Desc:    mults the current state matrix
	//Params:  matrix - matrix to mult with
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::multMatrix(const Mat4 &matrix) {
		glMultMatrixf(matrix);
	}

	//---------------------------------------------------------------------------
	//Desc:    loads the current state matrix
	//Params:  matrix - matrix to load
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::loadMatrix(const Mat4 &matrix) {
		glLoadMatrixf(matrix);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the current matrix mode
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::setMatrixMode_Projection() {
		glMatrixMode(GL_PROJECTION);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the current matrix mode
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::setMatrixMode_Modelview() {
		glMatrixMode(GL_MODELVIEW);
	}

	//---------------------------------------------------------------------------
	//Desc:    pushes the state matrix stack
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::push() {
		glPushMatrix();
	}

	//---------------------------------------------------------------------------
	//Desc:    pops the state matrix stack
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::pop() {
		glPopMatrix();
	}

	//---------------------------------------------------------------------------
	//Desc:    replases the current state matrix with identity matrix
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::identity() {
		glLoadIdentity();
	}

	//---------------------------------------------------------------------------
	//Desc:    mults the current state matrix with translation matrix
	//Params:  pos - position to translate
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::translate(const Vec3 &pos) {
		glTranslatef(pos.x, pos.y, pos.z);
	}

	//---------------------------------------------------------------------------
	//Desc:    mults the current state matrix with rotation matrix
	//Params:  angle - rotation angle, axis - rotation angle
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::rotate(float angle, const Vec3 &axis) {
		glRotatef(angle, axis.x, axis.y, axis.z);
	}

	//---------------------------------------------------------------------------
	//Desc:    mults the current state matrix with scale matrix
	//Params:  coef - scale coefs
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::scale(const Vec3 &coef) {
		glScalef(coef.x, coef.y, coef.z);
	}



	//---------------------------------------------------------------------------
	//Desc:    sets the current state color
	//Params:  color - new color
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::setColor(const Vec3 &color) {
		glColor3fv(color);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the current state color
	//Params:  color - new color
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::setColor(const Vec4 &color) {
		glColor4fv(color);
	}




	//---------------------------------------------------------------------------
	//Desc:    sets the current state cull function
	//Params:  type - cull type
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::cullFunc(CullType type) {
		glFrontFace(type);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the current state cull face
	//Params:  face - face to leave
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::cullFace(CullFace face) {
		glFrontFace(face);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables back face culling
	//Params:  type - cull type
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableCulling(CullType type) {
		glEnable(GL_CULL_FACE);
		glFrontFace(type);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables back face culling
	//Params:  face - face to leave
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableCulling(CullFace face) {
		glEnable(GL_CULL_FACE);
		glFrontFace(face);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables back face culling
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableCulling() {
		glEnable(GL_CULL_FACE);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables back face culling
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disableCulling() {
		glDisable(GL_CULL_FACE);
	}




	//---------------------------------------------------------------------------
	//Desc:    sets the current state alpha test function
	//Params:  type - alpha test function
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::alphaTestFunc(CompareType type, float alphaRef) {
		glAlphaFunc(type, alphaRef);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the alpha test
	//Params:  type - alpha test function
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableAlphaTest(CompareType type, float alphaRef) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(type, alphaRef);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the alpha test
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableAlphaTest() {
		glEnable(GL_ALPHA_TEST);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables the depth test
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disableAlphaTest() {
		glDisable(GL_ALPHA_TEST);
	}





	//---------------------------------------------------------------------------
	//Desc:    sets the current state depth test function
	//Params:  type - depth test function
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::depthFunc(CompareType type) {
		glDepthFunc(type);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the depth test
	//Params:  type - depth test function
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableDepth(CompareType type) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(type);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the depth test
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableDepth() {
		glEnable(GL_DEPTH_TEST);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables the depth test
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disableDepth() {
		glDisable(GL_DEPTH_TEST);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the depth buffer mask
	//Params:  mask - if false the depth buffer will be locked
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::depthMask(bool mask) {
		glDepthMask(mask);
	}




	void GLSystem::scissorRect(int x, int y, int z, int w) {
		glScissor(x, y, z, w);
	}

	void GLSystem::enableScissor(int x, int y, int z, int w) {
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, z, w);
	}

	void GLSystem::enableScissor() {
		glEnable(GL_SCISSOR_TEST);
	}

	void GLSystem::disableScissor() {
		glDisable(GL_SCISSOR_TEST);
	}




	//---------------------------------------------------------------------------
	//Desc:    sets the polygon offset function
	//Params:  a - offset factor, b - offset units
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::polygonOffsetFill(float a, float b) {
		glPolygonOffset(a, b);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the polygon offset fill
	//Params:  a - offset factor, b - offset units
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enablePolygonOffsetFill(float a, float b) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(a, b);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables the polygon offset fill
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enablePolygonOffsetFill() {
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables the polygon offset fill
	//Params:  a - offset factor, b - offset units
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disablePolygonOffsetFill() {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}



	//---------------------------------------------------------------------------
	//Desc:    enables blending
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableBlending() {
		glEnable(GL_BLEND);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the blending fuction
	//Params:  src - src color facrot, dst - dst color factor
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::blendFunc(BlendParam src, BlendParam dst) {
		glBlendFunc(src, dst);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables blending
	//Params:  src - src color facrot, dst - dst color factor
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableBlending(BlendParam src, BlendParam dst) {
		glEnable(GL_BLEND);
		glBlendFunc(src, dst);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables blending
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disableBlending() {
		glDisable(GL_BLEND);
	}




	//---------------------------------------------------------------------------
	//Desc:    sets clip plain
	//Params:  plain - plain eQuation, plainNum - plain number (max - 6)
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::clipPlane(const Vec4 &plain, int plainNum) {
		double eq[4] = { (double)plain[0], (double)plain[1], (double)plain[2], (double)plain[3] };

		glClipPlane(GL_CLIP_PLANE0 + plainNum, eq);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables clip plain
	//Params:  plainNum - plain number (max - 6)
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableClipPlane(int plainNum) {
		glEnable(GL_CLIP_PLANE0 + plainNum);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables and sets clip plain
	//Params:  plain - plain eQuation, plainNum - plain number (max - 6)
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enableClipPlane(const Vec4 &plain, int plainNum) {
		double eq[4] = { (double)plain[0], (double)plain[1], (double)plain[2], (double)plain[3] };

		glClipPlane(GL_CLIP_PLANE0 + plainNum, eq);
		glEnable(GL_CLIP_PLANE0 + plainNum);
	}

	//---------------------------------------------------------------------------
	//Desc:    disables clip plain
	//Params:  plainNum - plain number (max - 6)
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::disableClipPlane(int plainNum) {
		glDisable(GL_CLIP_PLANE0 + plainNum);
	}

	//---------------------------------------------------------------------------
	//Desc:    enables 2D mode
	//Params:  normalized - if true the ortho matrix will be normalized
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enable2d(bool normalized) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		if(normalized) {
			loadMatrix(Mat4::ortho(0, 1, 1, 0, 0, 1));
		} else {
			loadMatrix(Mat4::ortho(0, engine.iWindow->getWidth(), engine.iWindow->getHeight(), 0, 0, 1));
		}

		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();	
		glLoadIdentity();
	}

	//---------------------------------------------------------------------------
	//Desc:    enables 3D mode
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::enable3d() {	
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);	
		glPopMatrix();	
	}

	//---------------------------------------------------------------------------
	//Desc:    draws 2D rectangle
	//Params:  x0, y0, x3, y3 - min and max vertex coords, tx0, ty0, tx3, ty3 - tex coords 
	//Returns: -
	//---------------------------------------------------------------------------
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

	//---------------------------------------------------------------------------
	//Desc:    draws indexed geometry
	//Params:  index count - number of indices to draw
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::drawIndexedGeometry(void *indices, int indexCount) {
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
	}

	//---------------------------------------------------------------------------
	//Desc:    draws unindexed geometry
	//Params:  vertex count - number of indices to draw
	//Returns: -
	//---------------------------------------------------------------------------
	void GLSystem::drawGeometry(int vertexCount) {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}
