#pragma once


namespace NGEngine {

	struct CoreManager{
		CoreManager();

		virtual void quit()=0;
		virtual void initialise()=0;
		virtual void mainLoop()=0;
		
	public:
		bool running;
	public:
		struct IWindow*iWindow;
		struct IRender*iRender;
	};
}