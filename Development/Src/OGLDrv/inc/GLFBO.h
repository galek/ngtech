#pragma once 

//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	class GLFBO :public I_FBOManager {
	public:
		static GLFBO *create(int width, int height);
		void createColorAttachment();
		void createDepthAttachment();
		void createStencilAttachment();

		virtual ~GLFBO();

		void setColorTarget(I_Texture *texture = NULL, int face = -1);
		void setDepthTarget(I_Texture *texture = NULL);

		void set();

		/**
		Unsets GLFBO
		*/
		void unset();

		/**
		Clears the frame buffer
		*/
		void clear();

		/**
		Flushes the frame buffer
		*/
		void flush();
	};
}