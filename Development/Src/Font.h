/***************************************************************************
 *   Copyright (C) 2006 by AST   *
 *   tsyplyaev@gmail.com   *
 *   ICQ: 279-533-134                          *
 *   This is a part of work done by AST.       *
 *   If you want to use it, please contact me. *
 ***************************************************************************/

#pragma once

//**************************************
#include "GLSystem.h"
//**************************************

namespace NGEngine {

class Font {
public:
	Font(const String &path);
	~Font();
	void print(int x, int y, int size, const String &text, const Vec3 &color, float alpha);
		
private:
	GLTexture *fontTex;
	GLDisplayList *list[256];
};

};

