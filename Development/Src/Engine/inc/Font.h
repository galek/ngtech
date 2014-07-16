#pragma once

//**************************************
#include "GLSystem.h"
//**************************************

namespace NGTech {

	class ENGINE_API Font {
	public:
		explicit Font(const String &path);
		~Font();
		void print(int x, int y, int size, const String &text, const Vec3 &color, float alpha);

	private:
		GLTexture *fontTex;
		I_DisplayList *list[256];
	};
}