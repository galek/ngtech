#ifndef __GL_EXT_H__
#define __GL_EXT_H__

#ifdef _WIN32
#include <windows.h>
#endif

//#include "../../../Externals/Externs/GL/gl.h"
//#include "../../../Externals/Externs/GL/glext.h"

#ifdef USE_GL_WRAPPER
#include "GLWrapper.h"
#endif

//#include "Base.h"

/*
 */
class GLExt
{
	GLExt();
public:

	// initialize GLExt
	static int init();
	static int shutdown();

	// check OpenGL status
	static int isInitialized();

	// OpenGL multisample
	static void setMultisample(int multisample);
	static int getMultisample();

	// OpenGL viewport
	static void setViewport(int x, int y, int width, int height);
	static void getViewport(int *viewport);

	// OpenGL program
	static void setProgramID(GLuint program_id);
	static GLuint getProgramID();

	// OpenGL textures
	static void clearTextures();
	static void deleteTexture(GLuint target, GLuint texture_id);
	static void setTexture(int unit, GLuint target, GLuint texture_id);
	static void setTextureUnit(int unit, GLuint target, GLuint texture_id);
	static GLuint getTextureTarget(int unit);
	static GLuint getTextureID(int unit);

	// OpenGL synchronization
	static void waitSync(GLsync &sync);
	static void waitSync(GLsync sync[3]);
	static void waitSync(GLsync sync[3], int &offset, int flush, int size);
	static void fenceSync(GLsync sync[3], int offset, int flush, int size);

	// check OpenGL errors
	static int error(GLenum result);
	static int error();
};

/******************************************************************************\
*
* Structures
*
\******************************************************************************/

/*
 */
struct GLDrawArrays {
	GLuint count;
	GLuint instances;
	GLuint firstIndex;
	GLuint baseInstance;
};

/*
 */
struct GLDrawElements {
	GLuint count;
	GLuint instances;
	GLuint firstIndex;
	GLuint baseVertex;
	GLuint baseInstance;
};

#endif /* __GL_EXT_H__ */