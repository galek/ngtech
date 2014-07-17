/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/
#pragma once

//***************************************************************************
#include "IncludesAndLibs.h"
#include "../Common/EString.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc: GLExtensions
	//---------------------------------------------------------------------------
	class GLExtensions {
	public:
		static bool isExtSupported(const String &name);
		static void initExtensions();
	};
}

//---------------------------------------------------------------------------
//Desc: Multitexturing
//---------------------------------------------------------------------------
extern PFNGLACTIVETEXTUREPROC               glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREPROC         glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD1FPROC             glMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVPROC            glMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD2FPROC             glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC            glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD3FPROC             glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC            glMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD4FPROC             glMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC            glMultiTexCoord4fv;

//---------------------------------------------------------------------------
//Desc: VBO
//---------------------------------------------------------------------------
extern PFNGLBINDBUFFERARBPROC			 	   glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC		 	   glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC			 	   glGenBuffersARB;
extern PFNGLISBUFFERARBPROC			 		   glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC			 	   glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC		 	   glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC	 		   glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC			 	   glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC			 	   glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC 	   glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC	 	   glGetBufferPointervARB;

//---------------------------------------------------------------------------
//Desc: Occlusion
//---------------------------------------------------------------------------
extern PFNGLGENQUERIESARBPROC				   glGenQueriesARB;
extern PFNGLDELETEQUERIESARBPROC			   glDeleteQueriesARB;
extern PFNGLISQUERYARBPROC					   glIsQueryARB;
extern PFNGLBEGINQUERYARBPROC				   glBeginQueryARB;
extern PFNGLENDQUERYARBPROC					   glEndQueryARB;
extern PFNGLGETQUERYIVARBPROC				   glGetQueryivARB;
extern PFNGLGETQUERYOBJECTIVARBPROC			   glGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC		   glGetQueryObjectuivARB;

//---------------------------------------------------------------------------
//Desc: Textures
//---------------------------------------------------------------------------
extern PFNGLTEXIMAGE3DEXTPROC				   glTexImage3DEXT;
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC		   glCompressedTexImage3DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC		   glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC		   glCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC	   glCompressedTexSubImage3DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC	   glCompressedTexSubImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC	   glCompressedTexSubImage1DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC	   glGetCompressedTexImageARB;

//---------------------------------------------------------------------------
//Desc: GLSL
//---------------------------------------------------------------------------
extern PFNGLGETHANDLEARBPROC                   glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC                glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC                glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC               glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC                glAttachObjectARB;
extern PFNGLDELETEOBJECTARBPROC                glDeleteObjectARB;
extern PFNGLLINKPROGRAMARBPROC                 glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC             glValidateProgramARB;
extern PFNGLUNIFORM1FARBPROC                   glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC                   glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC                   glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC                   glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC                   glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC                   glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC                   glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC                   glUniform4iARB;
extern PFNGLUNIFORM1FVARBPROC                  glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC                  glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC                  glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC                  glUniform4fvARB;
extern PFNGLUNIFORM1IVARBPROC                  glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC                  glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC                  glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC                  glUniform4ivARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fvARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC                  glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC                glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC                glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC             glGetShaderSourceARB;

//---------------------------------------------------------------------------
//Desc: FBO
//---------------------------------------------------------------------------
extern PFNGLGENFRAMEBUFFERSEXTPROC			   glGenFramebuffersEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC		   glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC             glBindFramebufferEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC			   glGenRenderbuffersEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC		   glDeleteRenderbuffersEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC			   glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC		   glRenderbufferStorageEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	   glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC		   glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		   glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC		   glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	   glFramebufferRenderbufferEXT;