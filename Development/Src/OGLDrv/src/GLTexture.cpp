#include "RenderPrivate.h"
//***************************************************************************
#include "GLExtensions.h"
#include "GLTexture.h"
//***************************************************************************
//see http://www.slideshare.net/Mark_Kilgard/opengl-45-update-for-nvidia-gpus?qid=0ec7626a-1ec3-4178-aefb-2636607cf26e&v=qf1&b=&from_search=1
namespace NGTech {

	/**
	*/
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

	/**
	*/
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

	/**
	*/
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

	/**
	*/
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

	/**
	*/
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

	/**
	*/
	GLTexture *GLTexture::create2d(int width, int height, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, 1, TEXTURE_2D, format, (void**)data);
	}

	/**
	*/
	GLTexture *GLTexture::create3d(int width, int height, int depth, Format format) {
		GLubyte *data[1];
		data[0] = NULL;

		return create(width, height, depth, TEXTURE_3D, format, (void**)data);
	}

	/**
	*/
	GLTexture *GLTexture::createCube(int width, int height, Format format) {
		GLubyte *data[6];
		for (int i = 0; i < 6; i++) {
			data[i] = NULL;
		}

		return create(width, height, 1, TEXTURE_CUBE, format, (void**)data);
	}

	/**
	*/
	GLTexture::~GLTexture() {
		glDeleteTextures(1, &glID);
	}

	/**
	*/
	void GLTexture::setWrap(Wrap _wrap) {
		static const unsigned int Targets[] = {
			/*ZERO*/0,
			/*REPEAT*/0x2901,
			/*CLAMP*/0x2900,
			/*CLAMP_TO_EDGE*/0x812F
		};
		this->wrap = Targets[_wrap];

		glTexParameteri(target, GL_TEXTURE_WRAP_S, this->wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, this->wrap);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, this->wrap);
	}

	/**
	*/
	void GLTexture::setFilter(Filter filter) {

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
	}

	/**
	*/
	void GLTexture::setAniso(Aniso _aniso) {
		if (_aniso > 0) {
			this->aniso = _aniso;
			glTextureParameterfEXT(glID, target, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->aniso);
		}
	}

	/**
	*/
	void GLTexture::Set()
	{
		glEnable(this->target);
		glBindTexture(this->target, glID);
	}

	/**
	*/
	void GLTexture::UnSet()
	{
		glBindTexture(this->target, 0);
		glDisable(this->target);
	}

	/**
	*/
	void GLTexture::set(size_t tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glEnable(target);
		glBindTexture(target, glID);
	}

	/**
	*/
	void GLTexture::unset(size_t tex_unit) {
		glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
		glBindTexture(target, 0);
		glDisable(target);
	}

	/**
	*/
	void GLTexture::beginRenderTo() {
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/**
	*/
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

	/**
	*/
	void GLTexture::endRenderTo() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, GetWindow()->getWidth(), GetWindow()->getHeight());
	}

	/**
	*/
	GLTexture *GLTexture::create(int width, int height, int depth, Target target, Format format, void **data)
	{
		static const GLuint Targets[] = {
			/*ZERO*/0,
			/*TEXTURE_2D*/0x0DE1,
			/*TEXTURE_3D*/ 0x806F,
			/*TEXTURE_CUBE*/0x8513
		};
		GLTexture *texture = new GLTexture();

		texture->selectedTarget = target;

		texture->width = width;
		texture->height = height;
		texture->depth = depth;

		texture->wrap = GL_REPEAT;
		texture->target = Targets[target];

		texture->_ActivateFormat(format);

		glGenTextures(1, &texture->glID);
		glBindTexture(texture->target, texture->glID);//IN DSA is not needed

		texture->_ActivateWrap((Wrap)texture->wrap);

		Aniso aniso = GetRender()->defAniso;
		if (aniso > 0) {
			texture->aniso = aniso;
			glTexParameterf(texture->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, texture->aniso);
		}

		Filter filter = GetRender()->defFilter;
		texture->setFilter(filter);
		texture->_ActivateTarget(data);
		glBindTexture(texture->target, 0);//IN DSA is not needed

		return texture;
	}

	/**
	*/
	void GLTexture::_ActivateTarget(void **data)
	{
		if ((this->target == GL_TEXTURE_1D) || (this->target == GL_TEXTURE_1D_ARRAY))
		{
			glTexImage1D(this->target, 0, this->internalFormat, this->width, 0, this->srcFormat, this->dataType, data[0]);
		}
		else if ((this->target == GL_TEXTURE_2D) || (this->target == GL_TEXTURE_2D_ARRAY))
		{
			glTexImage2D(this->target, 0, this->internalFormat, this->width, this->height, 0, this->srcFormat, this->dataType, data[0]);
		}
		else if ((this->target == GL_TEXTURE_3D) || (this->target == GL_TEXTURE_CUBE_MAP_ARRAY))
		{
			glTexImage3D(this->target, 0, this->internalFormat, this->width, this->height, this->depth, 0, this->srcFormat, this->dataType, data[0]);
		}
		else if (this->target == GL_TEXTURE_CUBE_MAP)
		{
			for (int i = 0; i < 6; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->internalFormat, this->width, this->height, 0, this->srcFormat, this->dataType, data[i]);
			}
		}
	}

	/**
	*/
	void GLTexture::_ActivateFormat(Format _format)
	{
		if (_format == RGB) {
			this->srcFormat = GL_RGB;
			this->internalFormat = GL_RGB8;
			this->dataType = GL_UNSIGNED_BYTE;
		}
		else if (_format == RGBA) {
			this->srcFormat = GL_RGBA;
			this->internalFormat = GL_RGBA8;
			this->dataType = GL_UNSIGNED_BYTE;
		}

		else if (_format == RGB_16) {
			this->srcFormat = GL_RGB;
			this->internalFormat = GL_RGB16;
			this->dataType = GL_UNSIGNED_INT;
		}
		else if (_format == RGBA_16) {
			this->srcFormat = GL_RGBA;
			this->internalFormat = GL_RGBA16;
			this->dataType = GL_UNSIGNED_INT;
		}

		else if (_format == RGB_FP16) {
			this->srcFormat = GL_RGB;
			this->internalFormat = GL_RGB16F;
			this->dataType = GL_FLOAT;
		}
		else if (_format == RGBA_FP16) {
			this->srcFormat = GL_RGBA;
			this->internalFormat = GL_RGBA16F;
			this->dataType = GL_FLOAT;
		}

		else if (_format == RGB_FP32) {
			this->srcFormat = GL_RGB;
			this->internalFormat = GL_RGB32F;
			this->dataType = GL_FLOAT;
		}
		else if (_format == RGBA_FP32) {
			this->srcFormat = GL_RGBA;
			this->internalFormat = GL_RGBA32F;
			this->dataType = GL_FLOAT;
		}
	}

	/**
	*/
	void GLTexture::_ActivateWrap(Wrap _wrap)
	{
		//IN DSA replaced on glTextureParameteri
		this->wrap = _wrap;
		glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->wrap);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->wrap);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_R, this->wrap);
	}

	/**
	*/
	void GLTexture::SetMinMipLevel(unsigned int level)
	{
		_minMipLevel = level;
		glTexParameteri(
			target,
			GL_TEXTURE_BASE_LEVEL,
			level
			);
	}

	/**
	*/
	void GLTexture::SetMaxMipLevel(unsigned int level)
	{
		_maxMipLevel = level;
		glTexParameteri(
			target,
			GL_TEXTURE_MAX_LEVEL,
			level
			);
	}
}