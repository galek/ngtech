#pragma once
#include "..\..\Common\Types.h"
#include "..\..\Common\IWindow.h"
#include "..\..\Common\IRender.h"
#include "..\..\Core\inc\BuildDefines.h"
#include "..\..\Core\inc\CoreManager.h"
#include "..\..\Core\inc\Log.h"
#include "..\..\Engine\inc\dlldef.h"
#include "..\..\Engine\inc\Engine.h"
#include "..\..\Engine\inc\EngineAppBase.h"
#include "..\..\Engine\inc\Scene.h"
#include "..\..\Engine\inc\WrappedScriptFunctions.h"
#include "..\..\ExampleGame\ExampleGame.h"

using namespace NGTech;

namespace EngineCLR {

	public ref class EngineCLR
	{
	public:
		EngineCLR(int _hwnd)
			:hwnd(_hwnd),
			mInited(false),
			engine(nullptr)
		{}

		~EngineCLR() {
			delete engine;
			engine = nullptr;
		}

		void EngineInit()
		{
			if (!engine)
				EngineStart(hwnd, new ExampleGame(), new RenderCallback(), new EventsCallback());
			MouseGrab(false);
			PauseEngine(true);
		}
		ENGINE_INLINE bool isInited()	{ return mInited; }

		ENGINE_INLINE void Update() {
			if (!engine)
				return;
			engine->Update();
		}

		void MouseGrab(bool _val)
		{
			if (GetWindow() && engine)
				GetWindow()->grabMouse(_val);
		}

		void Resize(int _w, int _h)	{
			GetRender()->reshape(_w, _h);
		}

		bool isMouseGrabed()
		{
			if (GetWindow() && engine)
				return GetWindow()->isMouseGrabed();
			return false;
		}

		void KeyDown(int _key)
		{
			if ((mGrabbed) && engine)
				GetWindow()->setKeyDown(_key);
		}

		ENGINE_INLINE void KeyUp(int _key)	{ GetWindow()->setKeyUp(_key); }

		void CameraSetDirection(int x, int y)
		{
			posX = x;
			posY = y;
			auto cam = GetScene()->GetCurrentCamera();
			if (cam && engine)
				cam->lookAt(posX, posY);
			else
				Warning("[Editor]Camera not exist");
		}
		ENGINE_INLINE void SetGrabbed(bool _s) { mGrabbed = _s; }
		ENGINE_INLINE bool isGrabbed() { return mGrabbed; }

		ENGINE_INLINE void SetShowCursor(bool _s) { GetWindow()->showOSCursor(_s); }

		ENGINE_INLINE void PauseEngine(bool _s) { API_PauseEngine(_s); }

		ENGINE_INLINE void LoadEngineFormat(System::String ^  _p) {
			std::string _a = "Empty";
			MarshalString(_p, _a);
			API_LoadEngineFormat(_a.c_str());
		}
	private:
		void EngineStart(int _hwnd, IGame*_game, ICallback *rc, ICallback *ev){
			if (!mInited && !engine)
			{
				engine = new EngineAppBase(_hwnd, true, _game, rc, ev);
				mInited = true;
			}
		}
	private:
		EngineAppBase* engine;
		int hwnd;
		bool mInited;
		bool mGrabbed;
		float posX, posY;
	};
}
