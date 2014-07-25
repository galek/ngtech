#pragma once

namespace NGTech {
	struct IGame;
	struct ICallback;

	struct ENGINE_API EngineAppBase {
		EngineAppBase(int _hwnd, IGame*_game, ICallback* rc, ICallback* ev);
		EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev);
		/**
		*/
		void Update();
	};
}