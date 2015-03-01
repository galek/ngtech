#include "CorePrivate.h"
#include "CvarManager.h"
#include "../../Common/StringHelper.h"
#include "Config.h"

namespace NGTech {
	//TODO:Сделать получение размер дисплея
	CVARManager::CVARManager(Config*_c)
		:
		//Render
		r_width(_c->getFloat("r_width", 1024.f)),
		r_height(_c->getFloat("r_height", 768)),
		r_bpp(_c->getFloat("r_bpp", 32.f)),
		r_zdepth(_c->getFloat("r_zdepth", 24.f)),
		r_fullscreen(_c->getBool("r_fullscreen", false)),
		r_showInfo(_c->getBool("r_DisplayInfo", false)),
		r_shadowsize(_c->getInt("r_shadowsize", 512)),
		r_shadowtype(_c->getInt("r_shadowtype", 4)),
		r_specular(_c->getBool("r_specular", true)),
		r_hdr(_c->getBool("r_hdr", true)),
		r_parallax(_c->getBool("r_parallax", true)),
		r_reflections(_c->getBool("r_reflections", true)),
		r_wireframe(_c->getBool("r_wireframe", false)),
		//Client
		cl_fov(_c->getFloat("cl_fov", 60.f)),
		//Window
		w_withoutBorder(_c->getBool("w_withoutBorder", false))
	{}
}