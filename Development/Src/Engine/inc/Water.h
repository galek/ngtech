#pragma once

//************************************
#include "../../Common/IRender.h"
//************************************

namespace NGTech {

	class Water {
	public:
		Water();
		~Water();

		void draw();

		ENGINE_INLINE void setSize(float size) { this->size = size; };
		ENGINE_INLINE float getSize() { return size; };

		ENGINE_INLINE void setDepth(float depth) { this->depth = depth; };
		ENGINE_INLINE float getDepth() { return depth; };

	private:
		I_Texture *dudvMap;
		I_Texture *normalMap;

		I_Shader *waterShader;

		float size;
		float depth;
	};
}