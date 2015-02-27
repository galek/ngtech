#pragma once 

//***************************************************************************
#include "MathLib.h"
//***************************************************************************

namespace NGTech {

	class GLFBO :public I_FBOManager 
	{
	public:
		virtual ~GLFBO();
	public:
		static GLFBO *create(int width, int height);
		virtual void createColorAttachment();
		virtual void createDepthAttachment();
		virtual void createStencilAttachment();
		
		virtual void setColorTarget(I_Texture *texture = NULL, int face = -1);
		virtual void setDepthTarget(I_Texture *texture = NULL);
		virtual void setShadowTarget(int target);

		virtual void set();

		/**
		Unsets GLFBO
		*/
		void unset();

		/**
		Clears the frame buffer
		*/
		void clear();
	};
}