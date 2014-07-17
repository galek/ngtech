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
#include "GLTexture.h"
#include "GLSystem.h"
#include "WindowSystem.h"
#include "Log.h"
//***************************************************************************

namespace NGTech {

	//---------------------------------------------------------------------------
	//Desc:    creates 2d GLTexture from file
	//Params:  path - image file path, wrap - texture edges wrap
	//Returns: pointer to new Texture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create2d(const String &path) {

		ILImage *image = ILImage::create2d(path);

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if(image->getFormat() == ILImage::RGB) {
			format = RGB;
		} else if(image->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(), 
			TEXTURE_2D, format, (void**)data);

		delete image;

		return texture;
	}

	//---------------------------------------------------------------------------
	//Desc:    adds GLTexture from 6 files
	//Params:  path - pointer to array of image file paths, wrap - texture edges wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::createCube(const String &path) {

		ILImage *image[6];	
		const char *suffix[] = { "px", "nx",  "py", "ny",  "pz", "nz" };

		for(int i = 0; i < 6; i++) {
			char dst[1024];
			sprintf(dst, path.c_str(), suffix[i]);
			image[i] = ILImage::create2d(dst);
		}
		GLubyte *data[6];
		for(int i = 0; i < 6; i++) 
			data[i] = image[i]->getData();
		
		Format format;
		if(image[0]->getFormat() == ILImage::RGB) 
			format = RGB;
		 else if(image[0]->getFormat() == ILImage::RGBA) 
			format = RGBA;

		GLTexture *texture = create(image[0]->getWidth(), image[0]->getHeight(), image[0]->getDepth(), 
			TEXTURE_CUBE, format, (void**)data);
		
		for(int i = 0; i < 6; i++) {
			delete image[i];
		}

		return texture;
	}

	//---------------------------------------------------------------------------
	//Desc:    creates 2d GLTexture from image
	//Params:  image - pointer to ILImage to use, wrap - texture edges wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create2d(I_ILImage *image) {

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if(image->getFormat() == ILImage::RGB) {
			format = RGB;
		} else if(image->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(), 
			TEXTURE_2D, format, (void**)data);


		return texture;
	}

	//---------------------------------------------------------------------------
	//Desc:    creates 3d GLTexture from image
	//Params:  image - pointer to ILImage to use, wrap - texture edges wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create3d(I_ILImage *image) {

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if(image->getFormat() == ILImage::RGB) {
			format = RGB;
		} else if(image->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(), 
			TEXTURE_3D, format, (void**)data);

		return texture;
	}

	//---------------------------------------------------------------------------
	//Desc:    creates GLTexture from 6 images
	//Params:  image - pointer to arrray of ILImages to use, wrap - texture edges wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::createCube(I_ILImage **image) {

		GLubyte *data[6];
		for(int i = 0; i < 6; i++) {
			data[i] = image[i]->getData();
		}

		Format format;
		if(image[0]->getFormat() == ILImage::RGB) {
			format = RGB;
		} else if(image[0]->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image[0]->getWidth(), image[0]->getHeight(), image[0]->getDepth(), 
			TEXTURE_CUBE, format, (void**)data);

		return texture;
	}

	//---------------------------------------------------------------------------
	//Desc:    creates empty 2d GLTexture
	//Params:  width, height - tex size, format - tex format, wrap - tex edge wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create2d(int width, int height, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, 1, TEXTURE_2D, format, (void**)data);
	}

	//---------------------------------------------------------------------------
	//Desc:    creates empty 3d GLTexture
	//Params:  width, height - tex size, format - tex format, wrap - tex edge wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create3d(int width, int height, int depth, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, depth, TEXTURE_3D, format, (void**)data);
	}

	//---------------------------------------------------------------------------
	//Desc:    creates empty CUBE GLTexture
	//Params:  width, height - tex size, format - tex format, wrap - tex edge wrap
	//Returns: pointer to new GLTexture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::createCube(int width, int height, Format format) {
		GLubyte *data[6];
		for(int i = 0; i < 6; i++) {
			data[i] = NULL;
		}

		return create(width, height, 1, TEXTURE_CUBE, format, (void**)data);
	}

	//---------------------------------------------------------------------------
	//Desc:    GLTexture destructor
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	GLTexture::~GLTexture() {
		glDeleteTextures(1, &glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets GLTexture wrap
	//Params:  wrap - wrap value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::setWrap(Wrap wrap) {
		this->wrap = wrap;

		glBindTexture(target, glID);

		glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);

		glBindTexture(target, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets GLTexture filter
	//Params:  filter - filter value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::setFilter(Filter filter) {

		glBindTexture(target, glID);

		if(filter == NEAREST) {
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		} else if(filter == LINEAR) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		} else if(filter == NEAREST_MIPMAP_NEAREST){
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST_MIPMAP_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		} else if(filter == LINEAR_MIPMAP_NEAREST) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		} else if(filter == LINEAR_MIPMAP_LINEAR) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}

		glBindTexture(target, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets GLTexture aniso
	//Params:  aniso - aniso value
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::setAniso(Aniso aniso) {
		glBindTexture(target, glID);

		if(aniso > 0) {
			this->aniso = aniso;
			glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->aniso);
		}

		glBindTexture(target, 0);
	}

	//---------------------------------------------------------------------------
	//Desc:    sets texture
	//Params:  tex_unit - texture unit number
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::set(int tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glEnable(target);
		glBindTexture(target, glID);
	}

	//---------------------------------------------------------------------------
	//Desc:    unsets texture
	//Params:  tex_unit - texture unit number
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::unset(int tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glBindTexture(target, 0);
		glDisable(target);
	}

	//---------------------------------------------------------------------------
	//Desc:    begins render to texture
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::beginRenderTo() {
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//---------------------------------------------------------------------------
	//Desc:    copies color buffer to CUBE texture
	//Params:  face - texture CUBE face to copy to
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::copy(int face) {
		glEnable(target);
		glBindTexture(target, glID);

		if(face < 0) {
			glCopyTexSubImage2D(target, 0, 0, 0, 0, 0, width, height);
		} else {
			glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face, 0, 0, 0, 0, 0, width, height);
		}

		glBindTexture(target, 0);
		glDisable(target);
	}

	//---------------------------------------------------------------------------
	//Desc:    ends render to texture
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void GLTexture::endRenderTo() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, GetWindow()->getWidth(), GetWindow()->getHeight());
	}

	//---------------------------------------------------------------------------
	//Desc:    creates new texture
	//Params:  width, height, depth - tex size, target - tex type, format - tex src format, wrap - tex edges wrap, filter - tex filter, aniso - tex aniso level, data - image data
	//Returns: pointer to new texture
	//---------------------------------------------------------------------------
	GLTexture *GLTexture::create(int width, int height, int depth, Target target, Format format, void **data) {
		GLTexture *texture =  new GLTexture();

		texture->width  = width;
		texture->height = height;
		texture->depth  = depth;

		texture->wrap   = GL_REPEAT;
		texture->target = target;

		if(format == RGB) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB8;
			texture->dataType = GL_UNSIGNED_BYTE;
		} else if(format == RGBA) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA8;
			texture->dataType = GL_UNSIGNED_BYTE;
		} 

		else if(format == RGB_16) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB16;
			texture->dataType = GL_UNSIGNED_INT;
		} else if(format == RGBA_16) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA16;
			texture->dataType = GL_UNSIGNED_INT;
		} 

		else if(format == RGB_FP16) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB16F_ARB;
			texture->dataType = GL_FLOAT;
		} else if(format == RGBA_FP16) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA16F_ARB;
			texture->dataType = GL_FLOAT;
		} 

		else if(format == RGB_FP32) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB32F_ARB;
			texture->dataType = GL_FLOAT;
		} else if(format == RGBA_FP32) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA32F_ARB;
			texture->dataType = GL_FLOAT;
		}

		glGenTextures(1, &texture->glID);
		glBindTexture(texture->target, texture->glID);

		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, texture->wrap);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, texture->wrap);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_R, texture->wrap);

		Aniso aniso  = GetRender()->defAniso;
		if(aniso > 0) {
			texture->aniso = aniso;
			glTexParameterf(texture->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, texture->aniso);
		}

		Filter filter = GetRender()->defFilter;
		if(filter == NEAREST) {
			texture->magFilter = GL_NEAREST;
			texture->minFilter = GL_NEAREST;
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		} else if(filter == LINEAR) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR;
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		} else if(filter == NEAREST_MIPMAP_NEAREST){
			texture->magFilter = GL_NEAREST;
			texture->minFilter = GL_NEAREST_MIPMAP_NEAREST;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		} else if(filter == LINEAR_MIPMAP_NEAREST) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR_MIPMAP_NEAREST;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		} else if(filter == LINEAR_MIPMAP_LINEAR) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR_MIPMAP_LINEAR;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}

		if(texture->target == GL_TEXTURE_2D) {
			glTexImage2D(texture->target, 0, texture->internalFormat, texture->width, texture->height, 0, texture->srcFormat, texture->dataType, data[0]);
		} else if(texture->target == GL_TEXTURE_3D) {
			glTexImage3DEXT(texture->target, 0, texture->internalFormat, texture->width, texture->height, texture->depth, 0, texture->srcFormat, texture->dataType, data[0]);
		} else if(texture->target == GL_TEXTURE_CUBE_MAP_ARB) {
			for(int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, texture->internalFormat, texture->width, texture->height, 0, texture->srcFormat, texture->dataType, data[i]);
			}	
		}
		glBindTexture(texture->target, 0);

		return texture;
	}

}