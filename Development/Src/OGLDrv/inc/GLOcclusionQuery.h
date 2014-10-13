#pragma once 

//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	/**
	Occlusion query class
	*/
	class GLOcclusionQuery :public I_OcclusionQuery {
	public:
		GLOcclusionQuery();
		virtual ~GLOcclusionQuery();
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

		/** 
		clear query
		*/
		virtual void clear();

		/** render destroy
		*/
		virtual void destroy();
	};
}