#pragma once

//**************************************
#include "IRender.h"
//**************************************

namespace NGTech {

	class ENGINE_API Font {
	public:
		explicit Font(const String &path);
		~Font();
		void print(int x, int y, int size, const String &text, const Vec3 &color, float alpha);

	private:
		I_Texture *fontTex;
		I_DisplayList *list[256];
	};
}