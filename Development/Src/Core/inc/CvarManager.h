#pragma once 


namespace NGTech {

	class Config;
	struct CORE_API CVARManager
	{
		CVARManager(Config *c);

		//Render
		int r_shadowtype, r_shadowsize;
		bool r_fullscreen, r_showInfo, r_specular, r_hdr, r_wireframe, r_parallax, r_reflections;
		float r_width, r_height, r_bpp, r_zdepth;
		//Window
		bool w_withoutBorder;
		//Client
		float cl_fov;
	};
}