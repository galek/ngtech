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

	/**
	Occlusion query class
	*/
	class GLOcclusionQuery {
	public:
		GLOcclusionQuery();
		~GLOcclusionQuery();
		/**
		Begins rendering to query
		*/
		void beginRendering();

		/**
		End rendering to query
		*/
		void endRendering();

		/**
		Get number of passed samples
		\return number of passed samples
		*/
		unsigned int getResult();

	private:
		GLuint glID;
	};
}