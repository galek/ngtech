/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Font.h"
#include "Log.h"
//**************************************

namespace NGTech {

	Font::Font(const String &path) {
		fontTex = GLTexture::create2d(path);
		fontTex->setFilter(GLTexture::LINEAR);

		for(int x = 0; x < 16; x++) {
			for(int y = 0; y < 16; y++) {

				float cx = 0.0625f;						
				float cy = 0.0625f;						

				list[y*16 + x] = new GLDisplayList();
				list[y*16 + x]->beginBuild();

				GetRender()->drawRect(0, 0, 1, 1, x*cx, y*cy - 0.0625f, x*cx + 0.0625f, y*cy); 
				GetRender()->translate(Vec3(0.8, 0, 0));

				list[y*16 + x]->endBuild();
			}
		}
	}


	Font::~Font() {
		for(int i = 0; i < 256; i++) {
			delete list[i];
		}

		delete fontTex;
	}


	void Font::print(int x, int y, int size, const String &text, const Vec3 &color, float alpha) {
		GetRender()->enable2d(false);
		GetRender()->disableCulling();

		GetRender()->translate(Vec3(x, y, 0));								
		GetRender()->scale(Vec3(size, size, 1));

		GetRender()->enableBlending(GLSystem::ONE, GLSystem::ONE_MINUS_SRC_ALPHA);

		GetRender()->setColor(Vec4(color * alpha, alpha));
		fontTex->set(0);

		for(int p = 0; p < text.length(); p++) {
			list[text[p] + 16]->call();
		}

		fontTex->unset(0);
		GetRender()->setColor(Vec4(1, 1, 1, 1));

		GetRender()->disableBlending();

		GetRender()->enableCulling();
		GetRender()->enable3d();
	}

}


