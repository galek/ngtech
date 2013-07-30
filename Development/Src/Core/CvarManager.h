#pragma once 


namespace VEGA {

	class Config;
	struct CVARManager
	{
		CVARManager(Config *c);
		int width, height, bpp, zdepth, shadowtype;
		bool fullscreen, showInfo, use_specular;
	};
}