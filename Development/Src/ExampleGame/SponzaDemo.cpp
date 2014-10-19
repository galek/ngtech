#include "stdafx.h"
#include "..\..\API\NGTechEngineAPI.h"

#define SPONZA_DEMO

#ifdef SPONZA_DEMO
#include "MyGUI_Gui.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_Button.h"
using namespace MyGUI;
using namespace NGTech;

#include "GameGUI.h"

void ExampleGame::update() {}
//------------------------------------------------------------
void ExampleGame::initialise() {
	events = new GameGUIEvents();
	//initializing loading screen
	LoadingScreen lscreen("sponza/background.png");
	lscreen.show();

	sponza = new ObjectMesh("sponza.nggf");
	{
		sponza->setMaterial("arcs floor", "arcs_floor.mat");
		sponza->setMaterial("arcs 01", "arcs_01.mat");
		sponza->setMaterial("arcs 02", "arcs_02.mat");
		sponza->setMaterial("arcs 03", "arcs_03.mat");
		sponza->setMaterial("arcs 04", "arcs_04.mat");
		sponza->setMaterial("arcs floo0", "arcs_floo0.mat");
		sponza->setMaterial("arcs small", "arcs_small.mat");
		sponza->setMaterial("arcs long", "arcs_long.mat");


		sponza->setMaterial("object32", "object32.mat");
		sponza->setMaterial("object31", "object31.mat");
		sponza->setMaterial("pillar cor", "pillar_cor.mat");
		sponza->setMaterial("pillar flo", "pillar_flo.mat");
		sponza->setMaterial("object28", "object28.mat");
		sponza->setMaterial("object27", "object27.mat");
		sponza->setMaterial("pillar qua", "pillar_qua.mat");
		sponza->setMaterial("pillar rou", "pillar_rou.mat");
		sponza->setMaterial("puillar fl", "puillar_fl.mat");

		sponza->setMaterial("object19", "object19.mat");
		sponza->setMaterial("object21", "object21.mat");
		sponza->setMaterial("object23", "object23.mat");

		sponza->setMaterial("relief", "relief.mat");
		sponza->setMaterial("holes", "holes.mat");
		sponza->setMaterial("outside01", "outside01.mat");

		sponza->setMaterial("parapet", "parapet.mat");
		sponza->setMaterial("round hole", "round_hole.mat");
		sponza->setMaterial("ceiling", "ceiling.mat");
		sponza->setMaterial("object6", "object6.mat");
		sponza->setMaterial("object5", "object5.mat");
		sponza->setMaterial("object4", "object4.mat");
		sponza->setMaterial("object3", "object3.mat");
		sponza->setMaterial("walls", "walls.mat");
		sponza->setMaterial("windows", "windows.mat");
		sponza->setMaterial("doors", "doors.mat");

		sponza->setMaterial("floors", "floors.mat");
	}
	sponza->setTransform(Mat4::translate(Vec3(0, -10, 0)));
	sponza->setPhysicsStaticMesh();

	for (int i = 0; i < 5; i++) {
		box[i] = new ObjectMesh("cube.nggf");
		box[i]->setMaterial("*", "grid.mat");
		box[i]->setTransform(Mat4::translate(Vec3(-10 - i * 2, i * 20 + 10, i - 10)));
		box[i]->setPhysicsBox(Vec3(10, 10, 10), 10);
		box[i]->setImpactSound("impact.ogg");
	}

	for (int i = 0; i < 5; i++) {
		sphere[i] = new ObjectMesh("sphere.nggf");
		sphere[i]->setMaterial("*", "grid.mat");
		sphere[i]->setTransform(Mat4::translate(Vec3(10 + i * 2, i * 20 + 10, i - 10)));
		sphere[i]->setPhysicsSphere(Vec3(5, 5, 5), 10);
		sphere[i]->setImpactSound("impact.ogg");
	}

	for (int i = 0; i < 5; i++) {
		cylinder[i] = new ObjectMesh("torus.nggf");
		cylinder[i]->setMaterial("*", "grid.mat");
		cylinder[i]->setTransform(Mat4::translate(Vec3(20 + i * 2, i * 20 + 20, i - 10)));
		cylinder[i]->setPhysicsConvexHull(10.0f);
		cylinder[i]->setImpactSound("impact.ogg");
	}

	camera = new CameraFree();
	camera->setPosition(Vec3(0, 20, 0));
	camera->setMaxVelocity(2000);
	camera->setPhysics(Vec3(5, 5, 5), 1.0f);
	camera->setFOV(60);


	omniLight = new LightPoint();
	omniLight->setColor(Vec3(1, 1, 1));
	omniLight->setPosition(Vec3(0, 60, 0));
	omniLight->setRadius(200);

	particlesPink = new ParticleSystem("smoke.png", 50);
	particlesPink->setPosition(Vec3(60, 60, 0));
	particlesPink->setColor(Vec3(1, 0.5, 1));
	particlesPink->setForce(Vec3(-0.5, 1, -0.7));
	particlesPink->setVelocity(Vec3(-10, 0, 0));
	particlesPink->setParticleLifeTime(10000);
	particlesPink->setDispersion(0.1);

	particlesYellow = new ParticleSystem("smoke.png", 50);
	particlesYellow->setPosition(Vec3(-60, 60, 0));
	particlesYellow->setColor(Vec3(1, 1, 0.7));
	particlesYellow->setForce(Vec3(0.5, 1, 0.5));
	particlesYellow->setVelocity(Vec3(10, 0, 0));
	particlesYellow->setParticleLifeTime(10000);
	particlesYellow->setDispersion(0.1);

	//GetScene()->setAmbient(Vec3(0.2, 0.2, 0.2));

	GetWindow()->grabMouse(true);

	MyGUI::ButtonPtr button = GetGUI()->getGUI()->createWidget<MyGUI::Button>("Button", 10, 10, 300, 26, MyGUI::Align::Default, "Main");
	button->setFontName("DejaVuSansFont_15");
	button->setCaption("Exit");
	button->eventMouseButtonClick = MyGUI::newDelegate(events, &GameGUIEvents::ExitEvent);
}
//------------------------------------------------------------
EventsCallback::EventsCallback() : depth(10.0f){}
//------------------------------------------------------------

void ShowConsole()
{
	bool status = GetEngine()->ConsoleIsActive();
	GetEngine()->ConsoleShow(!status);
	GetWindow()->grabMouse(status);
}


void EventsCallback::Body(){
	if (GetWindow()->isKeyDown("ESC"))
		GetWindow()->toggleGrabMouse();

	if (GetWindow()->isKeyDown("EQUAL"))
	{
		ShowConsole();
	}
}
#endif