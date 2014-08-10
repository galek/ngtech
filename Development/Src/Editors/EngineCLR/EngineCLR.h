#pragma once
#include "..\..\Common\Types.h"
#include "..\..\Common\IWindow.h"
#include "..\..\Common\IRender.h"
#include "..\..\Core\inc\CoreManager.h"
#include "..\..\Engine\inc\dlldef.h"
#include "..\..\Engine\inc\Engine.h"
#include "..\..\Engine\inc\EngineAppBase.h"
#include "..\..\Engine\inc\Scene.h"
#include "..\..\ExampleGame\ExampleGame.h"

using namespace System;
using namespace NGTech;

namespace EngineCLR {

	public ref class EngineCLR
	{
	public:
		EngineCLR(int _hwnd)
			:hwnd(_hwnd),
			mInited(false),
			engine(nullptr)
		{
		}

		~EngineCLR() {
			delete engine;
			engine = nullptr;
		}

		void EngineInit()
		{
			if (!engine)
				EngineStart(hwnd, new ExampleGame(), new RenderCallback(), new EventsCallback());
			MouseGrab(false);
		}
		bool isInited()
		{
			return mInited;
		}

		void Update() {
			if (engine) {
				engine->Update();
			}
		}

		void MouseGrab(bool _val)
		{
			if (GetWindow())
				GetWindow()->grabMouse(_val);
		}

		void Resize(int _w, int _h)	{
			GetRender()->reshape(_w, _h);
		}

		bool isMouseGrabed()
		{
			if (GetWindow())
				return GetWindow()->isMouseGrabed();
			return false;
		}

		void CameraSetDirection(int x, int y)
		{
			posX = x;
			posY = y;
			auto cam = GetScene()->GetActiveCamera();
			if (cam)
				cam->LookAt(posX, posY);
			else
				Warning("[Editor]Camera not exist");
		}
		ENGINE_INLINE void SetGrabbed(bool _s) { mGrabbed = _s; }
		ENGINE_INLINE bool isGrabbed() { return mGrabbed; }

		ENGINE_INLINE void SetShowCursor(bool _s) { GetWindow()->showOSCursor(_s); }

		ENGINE_INLINE void CameraPlusVector(float _x, float _y, float _z) { GetScene()->GetActiveCamera()->setPosition(GetScene()->GetActiveCamera()->getPosition() += Vec3(_x, _y, _z)); }
	private:
		void EngineStart(int _hwnd, IGame*_game, ICallback *rc, ICallback *ev){
			if (!mInited)
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
