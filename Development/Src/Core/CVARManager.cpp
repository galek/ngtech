#include "CorePrivate.h"
#include "CvarManager.h"
#include "..\Common\EString.h"
#include "Config.h"

namespace VEGA {
	CVARManager::CVARManager(Config*_c)
		:width(_c->getInt("window_width")),
		height(_c->getInt("window_height")),
		bpp(_c->getInt("window_bpp")),
		zdepth(_c->getInt("window_zdepth")),
		fullscreen(_c->getBool("window_fullscreen")),
		showInfo(_c->getBool("r_DisplayInfo")),
		shadowtype(_c->getInt("light_shadowtype")),
		use_specular(_c->getBool("light_specular"))
	{}
}