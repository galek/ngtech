#include "RenderPrivate.h"
//***************************************************************************
#include "Engine.h"
#include "GLSystem.h"
#include "GLFBO.h"
#include "WindowSystem.h"

#include "Log.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates GLFBO
	//Params:  width - fbo width, height - fbo height, depthFormat - depth renderbuffer format
	//Returns: pointer to new GLFBO
	//---------------------------------------------------------------------------
	GLFBO *GLFBO::create(int width, int height) {
		GLFBO * fbo = new GLFBO();

		fbo->width = width;
		fbo->height = height;

		fbo->glColorID = fbo->glStencilID = fbo->glDepthID = 0;
		fbo->colorTarget = fbo->depthTarget = NULL;

		glGenFramebuffers(1, &fbo->glID);

		return fbo;
	}

	/**
	*/
	void GLFBO::createColorAttachment() {
		//Заполняем GBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, glID);

		glGenTextures(1, &glColorID);
		glBindTexture(GL_TEXTURE_RECTANGLE, glColorID);
		
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//В оригинале было glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);,а не ниже написанное
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, glColorID, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    adds depth attachment to GLFBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::createDepthAttachment() {
		glBindFramebuffer(GL_FRAMEBUFFER, glID);

		glGenRenderbuffersEXT(1, &glDepthID);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, glDepthID);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    adds stencil attachment to GLFBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::createStencilAttachment() {
		glBindFramebuffer(GL_FRAMEBUFFER, glID);

		glGenRenderbuffersEXT(1, &glStencilID);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, glStencilID);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    GLFBO deconstructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLFBO::~GLFBO() {
		glDeleteFramebuffersEXT(1, &glID);
		if (glColorID) glDeleteRenderbuffersEXT(1, &glColorID);
		if (glDepthID) glDeleteRenderbuffersEXT(1, &glDepthID);
		if (glStencilID) glDeleteRenderbuffersEXT(1, &glStencilID);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets texture as color attachment
	//Params:  texture - texture to set, face - cube map face
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::setColorTarget(I_Texture *texture, int face) {
		if (texture) {
			if (face < 0) {
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texture->target, texture->glID, 0);
			}
			else {
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face, texture->glID, 0);
			}
			colorTarget = texture;
		}
		else {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, glColorID, 0);
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets texture as depth attachment
	//Params:  texture - texture to set
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::setDepthTarget(I_Texture *texture) {
		if (texture) {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, texture->target, texture->glID, 0);
			depthTarget = texture;
		}
		else {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, GL_RENDERBUFFER_EXT, glDepthID);
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    sets GLFBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::set() {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, glID);

		if (glColorID && !colorTarget) glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, glColorID, 0);
		if (glDepthID && !depthTarget) glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, glDepthID);
		if (glStencilID) glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, glStencilID);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if (status == GL_FRAMEBUFFER_UNSUPPORTED_EXT) {
			Log::write("GLFBO::set() error: framebuffer unsupported");
		}

		glViewport(0, 0, width, height);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets GLFBO
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::unset() {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glViewport(0, 0, GetWindow()->getWidth(), GetWindow()->getHeight());
	}

	//---------------------------------------------------------------------------
	//Desc:    clears the frame buffer
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	//---------------------------------------------------------------------------
	//Desc:    flushes the frame buffer
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLFBO::flush() {
		glFlush();
	}

}