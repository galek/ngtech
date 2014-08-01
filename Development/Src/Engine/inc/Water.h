#pragma once

//************************************
#include "IRender.h"
//************************************

namespace NGTech {

	class Water {
	public:
		Water();
		~Water();

		void draw();

		void setSize(float size) { this->size = size; };
		float getSize() { return size; };

		void setDepth(float depth) { this->depth = depth; };
		float getDepth() { return depth; };

	private:
		I_Texture *dudvMap;
		I_Texture *normalMap;

		I_Shader *waterShader;

		float size;
		float depth;
	};
}