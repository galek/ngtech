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
		struct CVARManager*cvars;
		struct Log* log;
		class Config *config;
		class FileSystem*vfs;
		class ALSystem *alSystem;
		class ILSystem *ilSystem;
		class PhysSystem *physSystem;

		class Cash *cash;
		class GUI*gui;
		class Scene*scene;
		struct IGame*game;
	};
}