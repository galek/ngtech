/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//************************************
#include "IRender.h"
#include "ILSystem.h"
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