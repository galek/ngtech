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

		~GLFBO();

		void setColorTarget(I_Texture *texture = NULL, int face = -1);
		void setDepthTarget(I_Texture *texture = NULL);

		void set();
		void unset();

		void clear();
		void flush();

	private:
		GLuint glID;

		GLuint glColorID;
		GLuint glStencilID;
		GLuint glDepthID;

		I_Texture *colorTarget;
		I_Texture *depthTarget;

		int width, height;
	};
}