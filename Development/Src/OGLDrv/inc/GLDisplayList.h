#pragma once 

//***************************************************************************
#include "IncludesAndLibs.h"
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	class GLDisplayList :public I_DisplayList {
	public:
		GLDisplayList();
		virtual ~GLDisplayList();
		/**
		Begins the display list build
		*/
		virtual void beginBuild();

		/**
		Ends the display list build
		*/
		virtual void endBuild();

		/**
		Calls the display list
		*/
		virtual void call();
	};
}