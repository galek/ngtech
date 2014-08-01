/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "MyGUI_OpenGLPlatform.h"
#include <assert.h>

namespace MyGUI
{

	OpenGLPlatform::OpenGLPlatform() :
		mIsInitialise(false)
	{
		mRenderManager = new OpenGLRenderManager();
	}

	OpenGLPlatform::~OpenGLPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
	}

	void OpenGLPlatform::initialise(OpenGLImageLoader* _loader, const std::string& _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		mRenderManager->initialise(_loader);
	}

	void OpenGLPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
	}

	OpenGLRenderManager* OpenGLPlatform::getRenderManagerPtr()
	{
		assert(mIsInitialise);
		return mRenderManager;
	}

} // namespace MyGUI
