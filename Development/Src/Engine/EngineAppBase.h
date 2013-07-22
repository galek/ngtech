#pragma once

namespace VEGA {
	struct IGame;
	struct ICallback;

	struct VEGA_API EngineAppBase{
		EngineAppBase(IGame*_game, ICallback* rc, ICallback* ev);
		/**
		*/
		void Update();
	};
}