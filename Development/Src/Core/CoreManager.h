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
		struct IWindow*iWindow;
		struct IRender*iRender;
		struct CVARManager*cvars;
		struct Log* log;
		class Config *config;
		class FileSystem*vfs;
		class IAudio *alSystem;
		class ILSystem *ilSystem;
		class PhysSystem *physSystem;

		class Cache *cache;
		class GUI*gui;
		class Scene*scene;
		struct IGame*game;
	};
}