#pragma once

#include "..\Common\IGame.h"
//#define MANDELBROT_DEMO
//#define TERRAIN_DEMO



namespace MyGUI
{
	class TextBox;
}
namespace VEGA
{
	class ObjectMesh;
	class ObjectSkinnedMesh;
	class CameraFree;
	class LightOmni;
	class ParticleSystem;
}

//------------------------------------------------------------
class GAME_API ExampleGame : public VEGA::IGame
{
	virtual void initialise();
	virtual void update();

	//objects
	class VEGA::ObjectMesh *sponza;
	class VEGA::ObjectMesh *box[5];
	class VEGA::ObjectMesh *sphere[5];
	class VEGA::ObjectMesh *cylinder[5];
	class VEGA::ObjectSkinnedMesh *chammy;
	class VEGA::CameraFree *camera;
	class VEGA::LightOmni *omniLight;
	class MyGUI::TextBox *fpsLabel;

	class VEGA::ParticleSystem *particlesPink;
	class VEGA::ParticleSystem *particlesYellow;
};
//------------------------------------------------------------
class GAME_API RenderCallback : public VEGA::ICallback
{
	virtual void Body(){  };
};
//------------------------------------------------------------
class GAME_API EventsCallback : public VEGA::ICallback
{
public:
	EventsCallback();
	virtual void Body();
private:
	float depth;
};