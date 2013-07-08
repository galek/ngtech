#pragma once


namespace NGEngine {
	struct CoreManager{
		CoreManager();

		virtual void quit()=0;
		virtual void initialise()=0;
		virtual void mainLoop()=0;
		
		typedef void (*EngineCallback)();
	public:
		EngineCallback render_callback;
		EngineCallback events_callback;
		bool rc, ec;
		bool running;
	public:
		struct IWindow*iWindow;
		struct IRender*iRender;
	};
}