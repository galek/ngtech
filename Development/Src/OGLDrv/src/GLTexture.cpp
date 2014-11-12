#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
#include "GLTexture.h"
//***************************************************************************

namespace NGTech {

	GLTexture *GLTexture::create2d(const String &path) {

		I_ILImage *image = GetRender()->CreateImage2D(path);

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if (image->getFormat() == I_ILImage::RGB) {
			format = RGB;
		}
		else if (image->getFormat() == I_ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(),
			TEXTURE_2D, format, (void**)data);

		delete image;

		return texture;
	}

	GLTexture *GLTexture::createCube(const String &path) {

		I_ILImage *image[6];
		const char *suffix[] = { "px", "nx", "py", "ny", "pz", "nz" };

		for (int i = 0; i < 6; i++) {
			char dst[1024];
			sprintf(dst, path.c_str(), suffix[i]);
			image[i] = GetRender()->CreateImage2D(dst);
		}
		GLubyte *data[6];
		for (int i = 0; i < 6; i++)
			data[i] = image[i]->getData();

		Format format;
		if (image[0]->getFormat() == I_ILImage::RGB)
			format = RGB;
		else if (image[0]->getFormat() == I_ILImage::RGBA)
			format = RGBA;

		GLTexture *texture = create(image[0]->getWidth(), image[0]->getHeight(), image[0]->getDepth(),
			TEXTURE_CUBE, format, (void**)data);

		for (int i = 0; i < 6; i++)
			delete image[i];


		return texture;
	}

	GLTexture *GLTexture::create2d(I_ILImage *image) {

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if (image->getFormat() == ILImage::RGB) {
			format = RGB;
		}
		else if (image->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(),
			TEXTURE_2D, format, (void**)data);


		return texture;
	}

	GLTexture *GLTexture::create3d(I_ILImage *image) {

		GLubyte **data = new GLubyte*[1];
		data[0] = image->getData();

		Format format;
		if (image->getFormat() == ILImage::RGB) {
			format = RGB;
		}
		else if (image->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image->getWidth(), image->getHeight(), image->getDepth(),
			TEXTURE_3D, format, (void**)data);

		return texture;
	}

	GLTexture *GLTexture::createCube(I_ILImage **image) {

		GLubyte *data[6];
		for (int i = 0; i < 6; i++) {
			data[i] = image[i]->getData();
		}

		Format format;
		if (image[0]->getFormat() == ILImage::RGB) {
			format = RGB;
		}
		else if (image[0]->getFormat() == ILImage::RGBA) {
			format = RGBA;
		}

		GLTexture *texture = create(image[0]->getWidth(), image[0]->getHeight(), image[0]->getDepth(),
			TEXTURE_CUBE, format, (void**)data);

		return texture;
	}

	GLTexture *GLTexture::create2d(int width, int height, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, 1, TEXTURE_2D, format, (void**)data);
	}

	GLTexture *GLTexture::create3d(int width, int height, int depth, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, depth, TEXTURE_3D, format, (void**)data);
	}

	GLTexture *GLTexture::createCube(int width, int height, Format format) {
		GLubyte *data[6];
		for (int i = 0; i < 6; i++) {
			data[i] = NULL;
		}

		return create(width, height, 1, TEXTURE_CUBE, format, (void**)data);
	}

	GLTexture::~GLTexture() {
		glDeleteTextures(1, &glID);
	}

	void GLTexture::setWrap(Wrap _wrap) {
		this->wrap = _wrap;

		glBindTexture(target, glID);

		glTexParameteri(target, GL_TEXTURE_WRAP_S, this->wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, this->wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, this->wrap);

		glBindTexture(target, 0);
	}

	void GLTexture::setFilter(Filter filter) {

		glBindTexture(target, glID);

		if (filter == NEAREST) {
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		}
		else if (filter == LINEAR) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		}
		else if (filter == NEAREST_MIPMAP_NEAREST){
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST_MIPMAP_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}
		else if (filter == LINEAR_MIPMAP_NEAREST) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_NEAREST;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}
		else if (filter == LINEAR_MIPMAP_LINEAR) {
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
		}

		glBindTexture(target, 0);
	}

	void GLTexture::setAniso(Aniso _aniso) {
		glBindTexture(target, glID);

		if (_aniso > 0) {
			this->aniso = _aniso;
			glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->aniso);
		}

		glBindTexture(target, 0);
	}


	void GLTexture::Set()
	{
		glEnable(this->target);
		glBindTexture(this->target, glID);
	}

	void GLTexture::UnSet()
	{
		glBindTexture(this->target, 0);
		glDisable(this->target);
	}

	void GLTexture::set(size_t tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glEnable(target);
		glBindTexture(target, glID);
	}

	void GLTexture::unset(size_t tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glBindTexture(target, 0);
		glDisable(target);
	}

	void GLTexture::beginRenderTo() {
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLTexture::copy(int face) {
		glEnable(target);
		glBindTexture(target, glID);

		if (face < 0) {
			glCopyTexSubImage2D(target, 0, 0, 0, 0, 0, width, height);
		}
		else {
			glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + face, 0, 0, 0, 0, 0, width, height);
		}

		glBindTexture(target, 0);
		glDisable(target);
	}

	void GLTexture::endRenderTo() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, GetWindow()->getWidth(), GetWindow()->getHeight());
	}

	GLTexture *GLTexture::create(int width, int height, int depth, Target target, Format format, void **data) {
		GLTexture *texture = new GLTexture();

		texture->width = width;
		texture->height = height;
		texture->depth = depth;

		texture->wrap = GL_REPEAT;
		texture->target = target;

		if (format == RGB) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB8;
			texture->dataType = GL_UNSIGNED_BYTE;
		}
		else if (format == RGBA) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA8;
			texture->dataType = GL_UNSIGNED_BYTE;
		}

		else if (format == RGB_16) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB16;
			texture->dataType = GL_UNSIGNED_INT;
		}
		else if (format == RGBA_16) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA16;
			texture->dataType = GL_UNSIGNED_INT;
		}

		else if (format == RGB_FP16) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB16F_ARB;
			texture->dataType = GL_FLOAT;
		}
		else if (format == RGBA_FP16) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA16F_ARB;
			texture->dataType = GL_FLOAT;
		}

		else if (format == RGB_FP32) {
			texture->srcFormat = GL_RGB;
			texture->internalFormat = GL_RGB32F_ARB;
			texture->dataType = GL_FLOAT;
		}
		else if (format == RGBA_FP32) {
			texture->srcFormat = GL_RGBA;
			texture->internalFormat = GL_RGBA32F_ARB;
			texture->dataType = GL_FLOAT;
		}

		glGenTextures(1, &texture->glID);
		glBindTexture(texture->target, texture->glID);

		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, texture->wrap);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, texture->wrap);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_R, texture->wrap);

		Aniso aniso = GetRender()->defAniso;
		if (aniso > 0) {
			texture->aniso = aniso;
			glTexParameterf(texture->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, texture->aniso);
		}

		Filter filter = GetRender()->defFilter;
		if (filter == NEAREST) {
			texture->magFilter = GL_NEAREST;
			texture->minFilter = GL_NEAREST;
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}
		else if (filter == LINEAR) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR;
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}
		else if (filter == NEAREST_MIPMAP_NEAREST){
			texture->magFilter = GL_NEAREST;
			texture->minFilter = GL_NEAREST_MIPMAP_NEAREST;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}
		else if (filter == LINEAR_MIPMAP_NEAREST) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR_MIPMAP_NEAREST;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}
		else if (filter == LINEAR_MIPMAP_LINEAR) {
			texture->magFilter = GL_LINEAR;
			texture->minFilter = GL_LINEAR_MIPMAP_LINEAR;
			glTexParameteri(texture->target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, texture->magFilter);
			glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, texture->minFilter);
		}

		if (texture->target == GL_TEXTURE_2D) {
			glTexImage2D(texture->target, 0, texture->internalFormat, texture->width, texture->height, 0, texture->srcFormat, texture->dataType, data[0]);
		}
		else if (texture->target == GL_TEXTURE_3D) {
			glTexImage3DEXT(texture->target, 0, texture->internalFormat, texture->width, texture->height, texture->depth, 0, texture->srcFormat, texture->dataType, data[0]);
		}
		else if (texture->target == GL_TEXTURE_CUBE_MAP_ARB) {
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + i, 0, texture->internalFormat, texture->width, texture->height, 0, texture->srcFormat, texture->dataType, data[i]);
			}
		}
		glBindTexture(texture->target, 0);

		return texture;
	}

}