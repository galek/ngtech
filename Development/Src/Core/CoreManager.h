#pragma once


namespace VEGA {

	struct CoreManager{
		CoreManager();

		virtual void quit()=0;
		virtual void initialise()=0;
		virtual void mainLoop()=0;
		
	public:
		bool running;
	public:
		class ThreadPool* mTP;
		struct IWindow*iWindow;
		struct IRender*iRender;
	};
}