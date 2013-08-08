#pragma once 


namespace VEGA {

	class Config;
	struct CVARManager
	{
		CVARManager(Config *c);
		int r_width, r_height, r_bpp, r_zdepth, r_shadowtype,r_shadowsize;
		bool r_fullscreen, r_showInfo, r_specular, r_hdr,r_wireframe,r_parallax,r_reflections;
	};
}