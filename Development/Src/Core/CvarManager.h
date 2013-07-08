#pragma once 


namespace NGEngine {

	class Config;
	struct CVARManager
	{
		CVARManager(Config *c);
		int width, height, bpp, zdepth;
		bool fullscreen;
	};
}