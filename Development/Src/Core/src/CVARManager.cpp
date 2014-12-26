#include "CorePrivate.h"
#include "CvarManager.h"
#include "../../Common/StringHelper.h"
#include "Config.h"

namespace NGTech {
	CVARManager::CVARManager(Config*_c)
		:r_width(_c->getInt("r_width")),
		r_height(_c->getInt("r_height")),
		r_bpp(_c->getInt("r_bpp")),
		r_zdepth(_c->getInt("r_zdepth")),
		r_fullscreen(_c->getBool("r_fullscreen")),
		r_showInfo(_c->getBool("r_DisplayInfo")),
		r_shadowsize(_c->getInt("r_shadowsize")),
		r_shadowtype(_c->getInt("r_shadowtype")),
		r_specular(_c->getBool("r_specular")),
		r_hdr(_c->getBool("r_hdr")),
		r_parallax(_c->getBool("r_parallax")),
		r_reflections(_c->getBool("r_reflections")),
		r_wireframe(_c->getBool("r_wireframe"))
	{}
}