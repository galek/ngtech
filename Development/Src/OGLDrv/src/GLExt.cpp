#ifdef _LINUX
#include <GL/glx.h>
#elif _MACOS
#include <dlfcn.h>
#endif

#include "RenderPrivate.h"
//#include "glew/glew.h"


#include "GLFfp.h"
#include "GLExt.h"

using namespace NGTech;

/******************************************************************************\
*
* GLExt
*
\******************************************************************************/

/*
 */
#define GL_EXT_NUM_TEXTURES	32

/*
 */
static GLFfp *gl_ffp = NULL;

/*
 */
static int gl_multisample = 0;
static int gl_viewport[4] = { 0, };
static GLuint gl_program_id = 0;
static GLuint gl_texture_target[GL_EXT_NUM_TEXTURES] = { 0, };
static GLuint gl_texture_id[GL_EXT_NUM_TEXTURES] = { 0, };
static int gl_texture_unit = 0;

/*
 */
#ifndef USE_GL_WRAPPER
#ifdef _MACOS
static const char *gl_name = "/System/Library/Frameworks/OpenGL.framework/OpenGL";
static void *gl_handle = NULL;
static void *glGetProcAddress(const char *name) {
	if(gl_handle == NULL) gl_handle = dlopen(gl_name,RTLD_LAZY);
	if(gl_name != NULL) return dlsym(gl_handle,name);
	return NULL;
}
#endif
#endif

/*
 */
GLExt::GLExt() {

}

/*
 */
int GLExt::init() {

	assert(gl_ffp == NULL && "GLExt::init(): is already initialized");
	// current state
	gl_multisample = 0;
	gl_viewport[0] = 0;
	gl_viewport[1] = 0;
	gl_viewport[2] = 0;
	gl_viewport[3] = 0;
	gl_program_id = 0;
	clearTextures();

	gl_ffp = new GLFfp();

	return 1;
}

int GLExt::shutdown() {

	delete gl_ffp;
	gl_ffp = NULL;

	gl_multisample = 0;
	gl_viewport[0] = 0;
	gl_viewport[1] = 0;
	gl_viewport[2] = 0;
	gl_viewport[3] = 0;
	gl_program_id = 0;
	clearTextures();

	return 1;
}

/*
 */
int GLExt::isInitialized() {
	return (gl_ffp != NULL);
}

/*
 */
void GLExt::setMultisample(int multisample) {
	gl_multisample = multisample;
	if (gl_multisample) glEnable(GL_MULTISAMPLE);
	else glDisable(GL_MULTISAMPLE);
}

int GLExt::getMultisample() {
	return gl_multisample;
}

/*
 */
void GLExt::setViewport(int x, int y, int width, int height) {
	gl_viewport[0] = x;
	gl_viewport[1] = y;
	gl_viewport[2] = width;
	gl_viewport[3] = height;
	glViewport(x, y, width, height);
}

void GLExt::getViewport(int *viewport) {
	viewport[0] = gl_viewport[0];
	viewport[1] = gl_viewport[1];
	viewport[2] = gl_viewport[2];
	viewport[3] = gl_viewport[3];
}

/*
 */
void GLExt::setProgramID(GLuint program_id) {
	if (gl_program_id != program_id) {
		gl_program_id = program_id;
		glUseProgram(program_id);
	}
}

GLuint GLExt::getProgramID() {
	return gl_program_id;
}

/*
 */
void GLExt::clearTextures() {
	gl_texture_unit = 0;
	glActiveTexture(GL_TEXTURE0);
	for (int i = 0; i < GL_EXT_NUM_TEXTURES; i++) {
		gl_texture_target[i] = GL_TEXTURE_2D;
		gl_texture_id[i] = 0;
	}
}

void GLExt::deleteTexture(GLuint target, GLuint texture_id) {
	if (texture_id && glIsTexture(texture_id)) glDeleteTextures(1, &texture_id);
	for (int i = 0; i < GL_EXT_NUM_TEXTURES; i++) {
		if (gl_texture_id[i] == texture_id) {
			gl_texture_target[i] = GL_TEXTURE_2D;
			gl_texture_id[i] = 0;
			break;
		}
	}
}

void GLExt::setTexture(int unit, GLuint target, GLuint texture_id) {
	assert((unsigned int)unit < GL_EXT_NUM_TEXTURES && "GLExt::setTexture(): bad texture unit");
	if (gl_texture_id[unit] != texture_id) {
		gl_texture_target[unit] = target;
		gl_texture_id[unit] = texture_id;
		if (gl_texture_unit != unit) {
			gl_texture_unit = unit;
			glActiveTexture(GL_TEXTURE0 + unit);
		}
		glBindTexture(target, texture_id);
	}
}

void GLExt::setTextureUnit(int unit, GLuint target, GLuint texture_id) {
	assert((unsigned int)unit < GL_EXT_NUM_TEXTURES && "GLExt::setTextureUnit(): bad texture unit");
	if (gl_texture_unit != unit) {
		gl_texture_unit = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	if (gl_texture_id[unit] != texture_id) {
		gl_texture_target[unit] = target;
		gl_texture_id[unit] = texture_id;
		glBindTexture(target, texture_id);
	}
}

GLuint GLExt::getTextureTarget(int unit) {
	assert((unsigned int)unit < GL_EXT_NUM_TEXTURES && "GLExt::getTextureTarget(): bad texture unit");
	return gl_texture_target[unit];
}

GLuint GLExt::getTextureID(int unit) {
	assert((unsigned int)unit < GL_EXT_NUM_TEXTURES && "GLExt::getTextureID(): bad texture unit");
	return gl_texture_id[unit];
}

/*
 */
void GLExt::waitSync(GLsync &sync) {
	if (sync == NULL) return;
	int ret = glClientWaitSync(sync, 0, 0);
	while (ret != GL_ALREADY_SIGNALED && ret != GL_CONDITION_SATISFIED) {
		ret = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
		assert(ret != GL_WAIT_FAILED && "GLExt::waitSync(): waiting failed");
	}
	glDeleteSync(sync);
	sync = NULL;
}

void GLExt::waitSync(GLsync sync[3]) {
	if (sync[0]) waitSync(sync[0]);
	if (sync[1]) waitSync(sync[1]);
	if (sync[2]) waitSync(sync[2]);
}

void GLExt::waitSync(GLsync sync[3], int &offset, int flush, int size) {
	int size_3 = size / 3;
	int size_32 = size_3 * 2;
	if (offset + flush > size) {
		waitSync(sync[0]);
		offset = 0;
	}
	else if (offset <= size_3 && offset + flush > size_3) {
		waitSync(sync[1]);
		offset = size_3;
	}
	else if (offset <= size_32 && offset + flush > size_32) {
		waitSync(sync[2]);
		offset = size_32;
	}
}

void GLExt::fenceSync(GLsync sync[3], int offset, int flush, int size) {
	int size_3 = size / 3;
	int size_32 = size_3 * 2;
	if (offset == 0) {
		if (sync[2]) glDeleteSync(sync[2]);
		sync[2] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	else if (offset <= size_3 && offset + flush > size_3) {
		if (sync[0]) glDeleteSync(sync[0]);
		sync[0] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	else if (offset <= size_32 && offset + flush > size_32) {
		if (sync[1]) glDeleteSync(sync[1]);
		sync[1] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
}

/*
 */
int GLExt::error(GLenum result) {

	if (result == GL_NO_ERROR) return 0;

	if (result == GL_INVALID_ENUM) Log::error("OpenGL error: invalid enum\n");
	else if (result == GL_INVALID_VALUE) Log::error("OpenGL error: invalid value\n");
	else if (result == GL_INVALID_OPERATION) Log::error("OpenGL error: invalid operation\n");
	else if (result == GL_INVALID_FRAMEBUFFER_OPERATION) Log::error("OpenGL error: invalid framebuffer operation\n");
	else if (result == GL_STACK_OVERFLOW) Log::error("OpenGL error: stack overflow\n");
	else if (result == GL_STACK_UNDERFLOW) Log::error("OpenGL error: stack underflow\n");
	else if (result == GL_OUT_OF_MEMORY) Log::error("OpenGL error: out of memory\n");
	else /*Log::error*/Warning("OpenGL error: 0x%04X\n", result);

	return 1;
}

int GLExt::error() {
	int ret = 0;
	while (error(glGetError())) ret = 1;
	return ret;
}
