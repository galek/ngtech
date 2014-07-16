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
//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	class GLDisplayList :public I_DisplayList {
	public:
		GLDisplayList();
		~GLDisplayList();
		/**
		Begins the display list build
		*/
		void beginBuild();

		/**
		Ends the display list build
		*/
		void endBuild();

		/**
		Calls the display list
		*/
		void call();

	private:
		GLuint glID;
	};
}