#pragma once

#include "..\Common\IGame.h"
//#define MANDELBROT_DEMO
//#define TERRAIN_DEMO
#include "DLLx.h"



namespace MyGUI
{
	class TextBox;
}
namespace NGTech
{
	class ObjectMesh;
	class ObjectSkinnedMesh;
	class CameraFree;
	class LightPoint;
	class ParticleSystem;
}

//------------------------------------------------------------
class GAME_API ExampleGame : public NGTech::IGame
{
	virtual void initialise();
	virtual void update();
	virtual void render(){}

	//objects
	class NGTech::ObjectMesh *sponza;
	class NGTech::ObjectMesh *box[5];
	class NGTech::ObjectMesh *sphere[5];
	class NGTech::ObjectMesh *cylinder[5];
	class NGTech::ObjectMesh *chammy;
	class NGTech::CameraFree *camera;
	class NGTech::LightPoint *omniLight;
	class NGTech::ObjectSkinnedMesh * sMesh;
	class MyGUI::TextBox *fpsLabel;

	class NGTech::ParticleSystem *particlesPink;
	class NGTech::ParticleSystem *particlesYellow;
private:
	class GameGUIEvents*events;
};
//------------------------------------------------------------
class GAME_API RenderCallback : public NGTech::ICallback
{
	virtual void Body(){  };
};
//------------------------------------------------------------
class GAME_API EventsCallback : public NGTech::ICallback
{
public:
	EventsCallback();
	virtual void Body();
private:
	float depth;
};