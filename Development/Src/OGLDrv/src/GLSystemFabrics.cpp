#include "RenderPrivate.h"
//***************************************************************************
#include "Engine.h"
#include "Error.h"
#include "WindowSystem.h"
//***************************************************************************
#include "GLSystem.h"
#include "GLTexture.h"
#include "CommonDefs.h"
//***************************************************************************
#include "../../../Externals/glfw/include/GLFW/glfw3.h"
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
	GLShader  *GLSystem::ShaderCreateVSandFS(const String &pathFS, const String &pathVS, const String &defines){ return GLShader::createFromPath(pathFS, pathVS, defines); }

	/**
	*/
	GLFBO*GLSystem::CreateFBO(int x, int y){ return GLFBO::create(x, y); }

	/**
	*/
	GLVBO *GLSystem::CreateIBO(void *data, int numElements, int elemSize, int dataType){ return GLVBO::createIBO(data, numElements, elemSize, (I_VBManager::DataType)dataType); }

	/**
	*/
	GLVBO *GLSystem::CreateVBO(void *data, int numElements, int elemSize, int dataType, int drawType){ return GLVBO::createVBO(data, numElements, elemSize, (I_VBManager::DataType)dataType, (I_VBManager::TypeDraw)drawType); }

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

}