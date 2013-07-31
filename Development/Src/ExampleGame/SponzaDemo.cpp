//#include "EnginePrivate.h"
#include "stdafx.h"
#include "..\..\API\VegaEngineAPI.h"

#define SPONZA_DEMO

#ifdef SPONZA_DEMO
#include "MyGUI_Gui.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_Button.h"
using namespace MyGUI;
using namespace VEGA;


void ExampleGame::update() {}
//------------------------------------------------------------
void ExampleGame::initialise() {
	//initializing loading screen
	LoadingScreen *lscreen = LoadingScreen::create("../data/textures/logos/background.png");
	lscreen->show();

	sponza = new ObjectMesh("sponza.amdl");
	sponza->setMaterialList("sponza.amtrlst");
	sponza->setPhysicsStaticMesh();
	sponza->setTransform(Mat4::translate(Vec3(0, -10, 0)));

	for (int i = 0; i < 5; i++) {
		box[i] = new ObjectMesh("cube.amdl");
		box[i]->setMaterial("*", "grid.amtr");
		box[i]->setPhysicsBox(Vec3(10, 10, 10), 10);
		box[i]->setTransform(Mat4::translate(Vec3(-10 - i * 2, i * 20 + 10, i - 10)));
		box[i]->setImpactSound("impact.ogg");
	}
	for (int i = 0; i < 5; i++) {
		sphere[i] = new ObjectMesh("sphere.amdl");
		sphere[i]->setMaterial("*", "grid.amtr");
		sphere[i]->setPhysicsSphere(Vec3(5, 5, 5), 10);
		sphere[i]->setTransform(Mat4::translate(Vec3(10 + i * 2, i * 20 + 10, i - 10)));
		sphere[i]->setImpactSound("impact.ogg");
	}
	for (int i = 0; i < 5; i++) {
		cylinder[i] = new ObjectMesh("torus.amdl");
		cylinder[i]->setMaterial("*", "grid.amtr");
		cylinder[i]->setPhysicsConvexHull(10);
		cylinder[i]->setTransform(Mat4::translate(Vec3(20 + i * 2, i * 20 + 20, i - 10)));
		cylinder[i]->setImpactSound("impact.ogg");
	}

	camera = new CameraFree();
	camera->setPosition(Vec3(0, 20, 0));
	camera->setMaxVelocity(2000);
	camera->setPhysics(Vec3(5, 5, 5), 1.0);
	camera->setFOV(60);


	omniLight = new LightOmni();
	omniLight->setColor(Vec3(1, 1, 1));
	omniLight->setPosition(Vec3(0, 60, 0));
	omniLight->setRadius(200);

	particlesPink = new ParticleSystem("../data/textures/smoke.png", 50);
	particlesPink->setPosition(Vec3(60, 60, 0));
	particlesPink->setColor(Vec3(1, 0.5, 1));
	particlesPink->setForce(Vec3(-0.5, 1, -0.7));
	particlesPink->setVelocity(Vec3(-10, 0, 0));
	particlesPink->setParticleLifeTime(10000);
	particlesPink->setDispersion(0.1);

	particlesYellow = new ParticleSystem("../data/textures/smoke.png", 50);
	particlesYellow->setPosition(Vec3(-60, 60, 0));
	particlesYellow->setColor(Vec3(1, 1, 0.7));
	particlesYellow->setForce(Vec3(0.5, 1, 0.5));
	particlesYellow->setVelocity(Vec3(10, 0, 0));
	particlesYellow->setParticleLifeTime(10000);
	particlesYellow->setDispersion(0.1);

	GetEngine()->scene->setGravity(Vec3(0, -9.8, 0));
	GetEngine()->scene->setWater(1, 400);
	GetEngine()->scene->setAmbient(Vec3(0.2, 0.2, 0.2));

	GetEngine()->iWindow->grabMouse(true);

	MyGUI::ButtonPtr button = GetEngine()->gui->getGUI()->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
	button->setFontName("DejaVuSansFont.15");
	button->setCaption("Hello World!");
}
//------------------------------------------------------------
EventsCallback::EventsCallback() : depth(10.0f){}
//------------------------------------------------------------
void EventsCallback::Body(){

	if (GetEngine()->iWindow->wasKeyPressed(IWindow::KEY_ESC))
		GetEngine()->iWindow->toggleGrabMouse();

	if (GetEngine()->iWindow->isKeyPressed(IWindow::KEY_Z))
		GetEngine()->scene->setWater(depth += 1, 500);


	if (GetEngine()->iWindow->isKeyPressed(IWindow::KEY_X))
		GetEngine()->scene->setWater(depth -= 1, 500);

}
#endif