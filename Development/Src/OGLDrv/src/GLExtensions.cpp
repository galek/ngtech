#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
//***************************************************************************

//---------------------------------------------------------------------------
//Desc: Multitexturing
//---------------------------------------------------------------------------
PFNGLACTIVETEXTUREPROC               glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREPROC         glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD1FPROC             glMultiTexCoord1f = NULL;
PFNGLMULTITEXCOORD1FVPROC            glMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD2FPROC             glMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVPROC            glMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD3FPROC             glMultiTexCoord3f = NULL;
PFNGLMULTITEXCOORD3FVPROC            glMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD4FPROC             glMultiTexCoord4f = NULL;
PFNGLMULTITEXCOORD4FVPROC            glMultiTexCoord4fv = NULL;

//---------------------------------------------------------------------------
//Desc: VBO
//---------------------------------------------------------------------------
PFNGLBINDBUFFERARBPROC			 	   glBindBufferARB = NULL;
PFNGLDELETEBUFFERSARBPROC		 	   glDeleteBuffersARB = NULL;
PFNGLGENBUFFERSARBPROC			 	   glGenBuffersARB = NULL;
PFNGLISBUFFERARBPROC			 	   glIsBufferARB = NULL;
PFNGLBUFFERDATAARBPROC			 	   glBufferDataARB = NULL;
PFNGLBUFFERSUBDATAARBPROC		 	   glBufferSubDataARB = NULL;
PFNGLGETBUFFERSUBDATAARBPROC	 	   glGetBufferSubDataARB = NULL;
PFNGLMAPBUFFERARBPROC			 	   glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC			 	   glUnmapBufferARB = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC 	   glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC	 	   glGetBufferPointervARB = NULL;

//---------------------------------------------------------------------------
//Desc: Occlusion
//---------------------------------------------------------------------------
PFNGLGENQUERIESARBPROC				   glGenQueriesARB = NULL;
PFNGLDELETEQUERIESARBPROC			   glDeleteQueriesARB = NULL;
PFNGLISQUERYARBPROC					   glIsQueryARB = NULL;
PFNGLBEGINQUERYARBPROC				   glBeginQueryARB = NULL;
PFNGLENDQUERYARBPROC				   glEndQueryARB = NULL;
PFNGLGETQUERYIVARBPROC				   glGetQueryivARB = NULL;
PFNGLGETQUERYOBJECTIVARBPROC		   glGetQueryObjectivARB = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC		   glGetQueryObjectuivARB = NULL;

//---------------------------------------------------------------------------
//Desc: Textures
//---------------------------------------------------------------------------
PFNGLTEXIMAGE3DEXTPROC				   glTexImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC	   glCompressedTexImage3DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC	   glCompressedTexImage2DARB = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DARBPROC	   glCompressedTexImage1DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC	   glCompressedTexSubImage3DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC	   glCompressedTexSubImage2DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC	   glCompressedTexSubImage1DARB = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC	   glGetCompressedTexImageARB = NULL;

//---------------------------------------------------------------------------
//Desc: GLSL
//---------------------------------------------------------------------------
PFNGLGETHANDLEARBPROC                   glGetHandleARB = NULL;
PFNGLDETACHOBJECTARBPROC                glDetachObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC                glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC               glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC                glAttachObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC                glDeleteObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC                 glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObjectARB = NULL;
PFNGLVALIDATEPROGRAMARBPROC             glValidateProgramARB = NULL;
PFNGLUNIFORM1FARBPROC                   glUniform1fARB = NULL;
PFNGLUNIFORM2FARBPROC                   glUniform2fARB = NULL;
PFNGLUNIFORM3FARBPROC                   glUniform3fARB = NULL;
PFNGLUNIFORM4FARBPROC                   glUniform4fARB = NULL;
PFNGLUNIFORM1IARBPROC                   glUniform1iARB = NULL;
PFNGLUNIFORM2IARBPROC                   glUniform2iARB = NULL;
PFNGLUNIFORM3IARBPROC                   glUniform3iARB = NULL;
PFNGLUNIFORM4IARBPROC                   glUniform4iARB = NULL;
PFNGLUNIFORM1FVARBPROC                  glUniform1fvARB = NULL;
PFNGLUNIFORM2FVARBPROC                  glUniform2fvARB = NULL;
PFNGLUNIFORM3FVARBPROC                  glUniform3fvARB = NULL;
PFNGLUNIFORM4FVARBPROC                  glUniform4fvARB = NULL;
PFNGLUNIFORM1IVARBPROC                  glUniform1ivARB = NULL;
PFNGLUNIFORM2IVARBPROC                  glUniform2ivARB = NULL;
PFNGLUNIFORM3IVARBPROC                  glUniform3ivARB = NULL;
PFNGLUNIFORM4IVARBPROC                  glUniform4ivARB = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fvARB = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fvARB = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfvARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameterivARB = NULL;
PFNGLGETINFOLOGARBPROC                  glGetInfoLogARB = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjectsARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocationARB = NULL;
PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniformARB = NULL;
PFNGLGETUNIFORMFVARBPROC                glGetUniformfvARB = NULL;
PFNGLGETUNIFORMIVARBPROC                glGetUniformivARB = NULL;
PFNGLGETSHADERSOURCEARBPROC             glGetShaderSourceARB = NULL;

//---------------------------------------------------------------------------
//Desc: FBO
//---------------------------------------------------------------------------
PFNGLGENFRAMEBUFFERSEXTPROC			    glGenFramebuffersEXT = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC		    glDeleteFramebuffersEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC             glBindFramebufferEXT = NULL;
PFNGLGENRENDERBUFFERSEXTPROC			glGenRenderbuffersEXT = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC		    glDeleteRenderbuffersEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC			glBindRenderbufferEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC		    glRenderbufferStorageEXT;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	    glCheckFramebufferStatusEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC		glFramebufferTexture1DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC		glFramebufferTexture3DEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	    glFramebufferRenderbufferEXT = NULL;

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    checks if extension is supported
	//Params:  name - extension name
	//Returns: true if supported
	//---------------------------------------------------------------------------
	bool GLExtensions::isExtSupported(const String &name) {
		String ext = (char *)glGetString(GL_EXTENSIONS);

		for(int i = 0; i < StringHelper::getWordsNumber(ext); i++) {
			if(StringHelper::getWord(ext, i+1) == name) return true;
		}

		return false;
	}

	//---------------------------------------------------------------------------
	//Desc:    init all extensions
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
#ifdef WIN32
#define GET_PROC_ADDRESS(a, b) b = (a)wglGetProcAddress(#b)
#else
#define GET_PROC_ADDRESS(a, b) b = (a)glXGetProcAddressARB((const GLubyte*)#b)
#endif

	void GLExtensions::initExtensions() {
		//---------------------------------------------------------------------------
		//Desc: Multitexturing
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLACTIVETEXTUREPROC,       glActiveTextureARB);
		GET_PROC_ADDRESS(PFNGLCLIENTACTIVETEXTUREPROC, glClientActiveTextureARB);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1FPROC,     glMultiTexCoord1f);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1FVPROC,    glMultiTexCoord1fv);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2FPROC,     glMultiTexCoord2f);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2FVPROC,    glMultiTexCoord2fv);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3FPROC,     glMultiTexCoord3f);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3FVPROC,    glMultiTexCoord3fv);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4FPROC,     glMultiTexCoord4f);
		GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4FVPROC,    glMultiTexCoord4fv);

		//---------------------------------------------------------------------------
		//Desc: VBO
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLBINDBUFFERARBPROC,           glBindBufferARB);
		GET_PROC_ADDRESS(PFNGLDELETEBUFFERSARBPROC,        glDeleteBuffersARB);
		GET_PROC_ADDRESS(PFNGLGENBUFFERSARBPROC,           glGenBuffersARB);
		GET_PROC_ADDRESS(PFNGLISBUFFERARBPROC,             glIsBufferARB);
		GET_PROC_ADDRESS(PFNGLBUFFERDATAARBPROC,           glBufferDataARB);
		GET_PROC_ADDRESS(PFNGLBUFFERSUBDATAARBPROC,        glBufferSubDataARB);
		GET_PROC_ADDRESS(PFNGLGETBUFFERSUBDATAARBPROC,     glGetBufferSubDataARB);
		GET_PROC_ADDRESS(PFNGLMAPBUFFERARBPROC,            glMapBufferARB);
		GET_PROC_ADDRESS(PFNGLUNMAPBUFFERARBPROC,	      glUnmapBufferARB);
		GET_PROC_ADDRESS(PFNGLGETBUFFERPARAMETERIVARBPROC, glGetBufferParameterivARB);
		GET_PROC_ADDRESS(PFNGLGETBUFFERPOINTERVARBPROC,    glGetBufferPointervARB);

		//---------------------------------------------------------------------------
		//Desc: Occlusion
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLGENQUERIESARBPROC,        glGenQueriesARB);
		GET_PROC_ADDRESS(PFNGLDELETEQUERIESARBPROC,     glDeleteQueriesARB);
		GET_PROC_ADDRESS(PFNGLISQUERYARBPROC,           glIsQueryARB);
		GET_PROC_ADDRESS(PFNGLBEGINQUERYARBPROC,        glBeginQueryARB);
		GET_PROC_ADDRESS(PFNGLENDQUERYARBPROC,          glEndQueryARB);
		GET_PROC_ADDRESS(PFNGLGETQUERYIVARBPROC,        glGetQueryivARB);
		GET_PROC_ADDRESS(PFNGLGETQUERYOBJECTIVARBPROC,  glGetQueryObjectivARB);
		GET_PROC_ADDRESS(PFNGLGETQUERYOBJECTUIVARBPROC, glGetQueryObjectuivARB);

		//---------------------------------------------------------------------------
		//Desc: Textures
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLTEXIMAGE3DEXTPROC,              glTexImage3DEXT);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXIMAGE3DARBPROC,    glCompressedTexImage3DARB);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC,    glCompressedTexImage2DARB);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXIMAGE1DARBPROC,    glCompressedTexImage1DARB);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC, glCompressedTexSubImage3DARB);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC, glCompressedTexSubImage2DARB);
		GET_PROC_ADDRESS(PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC, glCompressedTexSubImage1DARB);
		GET_PROC_ADDRESS(PFNGLGETCOMPRESSEDTEXIMAGEARBPROC,   glGetCompressedTexImageARB);

		//---------------------------------------------------------------------------
		//Desc: GLSL
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLGETHANDLEARBPROC,            glGetHandleARB);
		GET_PROC_ADDRESS(PFNGLDETACHOBJECTARBPROC,         glDetachObjectARB);
		GET_PROC_ADDRESS(PFNGLCREATESHADEROBJECTARBPROC,   glCreateShaderObjectARB);
		GET_PROC_ADDRESS(PFNGLSHADERSOURCEARBPROC,         glShaderSourceARB);
		GET_PROC_ADDRESS(PFNGLCOMPILESHADERARBPROC,        glCompileShaderARB);
		GET_PROC_ADDRESS(PFNGLCREATEPROGRAMOBJECTARBPROC,  glCreateProgramObjectARB);
		GET_PROC_ADDRESS(PFNGLATTACHOBJECTARBPROC,         glAttachObjectARB);
		GET_PROC_ADDRESS(PFNGLDELETEOBJECTARBPROC,         glDeleteObjectARB);
		GET_PROC_ADDRESS(PFNGLLINKPROGRAMARBPROC,          glLinkProgramARB);
		GET_PROC_ADDRESS(PFNGLUSEPROGRAMOBJECTARBPROC,     glUseProgramObjectARB);
		GET_PROC_ADDRESS(PFNGLVALIDATEPROGRAMARBPROC,      glValidateProgramARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM1FARBPROC,            glUniform1fARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM2FARBPROC,            glUniform2fARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM3FARBPROC,            glUniform3fARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM4FARBPROC,            glUniform4fARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM1IARBPROC,            glUniform1iARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM2IARBPROC,            glUniform2iARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM3IARBPROC,            glUniform3iARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM4IARBPROC,            glUniform4iARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM1FVARBPROC,           glUniform1fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM2FVARBPROC,           glUniform2fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM3FVARBPROC,           glUniform3fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM4FVARBPROC,           glUniform4fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM1IVARBPROC,           glUniform1ivARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM2IVARBPROC,           glUniform2ivARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM3IVARBPROC,           glUniform3ivARB);
		GET_PROC_ADDRESS(PFNGLUNIFORM4IVARBPROC,           glUniform4ivARB);
		GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX2FVARBPROC,     glUniformMatrix2fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX3FVARBPROC,     glUniformMatrix3fvARB);
		GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX4FVARBPROC,     glUniformMatrix4fvARB);
		GET_PROC_ADDRESS(PFNGLGETOBJECTPARAMETERFVARBPROC, glGetObjectParameterfvARB);
		GET_PROC_ADDRESS(PFNGLGETOBJECTPARAMETERIVARBPROC, glGetObjectParameterivARB);
		GET_PROC_ADDRESS(PFNGLGETINFOLOGARBPROC,           glGetInfoLogARB);
		GET_PROC_ADDRESS(PFNGLGETATTACHEDOBJECTSARBPROC,   glGetAttachedObjectsARB);
		GET_PROC_ADDRESS(PFNGLGETUNIFORMLOCATIONARBPROC,   glGetUniformLocationARB);
		GET_PROC_ADDRESS(PFNGLGETACTIVEUNIFORMARBPROC,     glGetActiveUniformARB);
		GET_PROC_ADDRESS(PFNGLGETUNIFORMFVARBPROC,         glGetUniformfvARB);
		GET_PROC_ADDRESS(PFNGLGETUNIFORMIVARBPROC,         glGetUniformivARB);
		GET_PROC_ADDRESS(PFNGLGETSHADERSOURCEARBPROC,      glGetShaderSourceARB);

		//---------------------------------------------------------------------------
		//Desc: FBO
		//---------------------------------------------------------------------------
		GET_PROC_ADDRESS(PFNGLGENFRAMEBUFFERSEXTPROC,         glGenFramebuffersEXT);
		GET_PROC_ADDRESS(PFNGLDELETEFRAMEBUFFERSEXTPROC,      glDeleteFramebuffersEXT);
		GET_PROC_ADDRESS(PFNGLBINDFRAMEBUFFEREXTPROC,         glBindFramebufferEXT);
		GET_PROC_ADDRESS(PFNGLGENRENDERBUFFERSEXTPROC,        glGenRenderbuffersEXT);
		GET_PROC_ADDRESS(PFNGLDELETERENDERBUFFERSEXTPROC,     glDeleteRenderbuffersEXT);
		GET_PROC_ADDRESS(PFNGLBINDRENDERBUFFEREXTPROC,        glBindRenderbufferEXT);
		GET_PROC_ADDRESS(PFNGLRENDERBUFFERSTORAGEEXTPROC,     glRenderbufferStorageEXT);
		GET_PROC_ADDRESS(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC,  glCheckFramebufferStatusEXT);
		GET_PROC_ADDRESS(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC,    glFramebufferTexture1DEXT);
		GET_PROC_ADDRESS(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC,    glFramebufferTexture2DEXT);
		GET_PROC_ADDRESS(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC,    glFramebufferTexture3DEXT);
		GET_PROC_ADDRESS(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
	}

}
