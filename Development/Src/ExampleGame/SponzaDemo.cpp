#include "stdafx.h"
#include "../../API/NGTechEngineAPI.h"

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
#ifndef __LINUX__
	events = new GameGUIEvents();
	//initializing loading screen
	LoadingScreen lscreen("sponza/background.png");
	lscreen.show();

	sponza = new ObjectMesh("sponza.nggf");
	{
		sponza->setMaterial("arcs_floor.mat", "arcs floor");
		sponza->setMaterial("arcs_01.mat", "arcs 01");
		sponza->setMaterial("arcs_02.mat", "arcs 02");
		sponza->setMaterial("arcs_03.mat", "arcs 03");
		sponza->setMaterial("arcs_04.mat", "arcs 04");
		sponza->setMaterial("arcs_floo0.mat", "arcs floo0");
		sponza->setMaterial("arcs_small.mat", "arcs small");
		sponza->setMaterial("arcs_long.mat", "arcs long");

		sponza->setMaterial("object32.mat", "object32");
		sponza->setMaterial("object31.mat", "object31");
		sponza->setMaterial("pillar_cor.mat", "pillar cor");
		sponza->setMaterial("pillar_flo.mat", "pillar flo");
		sponza->setMaterial("object28.mat", "object28");
		sponza->setMaterial("object27.mat", "object27");
		sponza->setMaterial("pillar_qua.mat", "pillar qua");
		sponza->setMaterial("pillar_rou.mat", "pillar rou");
		sponza->setMaterial("puillar_fl.mat", "puillar fl");

		sponza->setMaterial("object19.mat", "object19");
		sponza->setMaterial("object21.mat", "object21");
		sponza->setMaterial("object23.mat", "object23");

		sponza->setMaterial("relief.mat", "relief");
		sponza->setMaterial("holes.mat", "holes");
		sponza->setMaterial("outside01.mat", "outside01");

		sponza->setMaterial("parapet.mat", "parapet");
		sponza->setMaterial("round_hole.mat", "round hole");
		sponza->setMaterial("ceiling.mat", "ceiling");
		sponza->setMaterial("object6.mat", "object6");
		sponza->setMaterial("object5.mat", "object5");
		sponza->setMaterial("object4.mat", "object4");
		sponza->setMaterial("object3.mat", "object3");
		sponza->setMaterial("walls.mat", "walls");
		sponza->setMaterial("windows.mat", "windows");
		sponza->setMaterial("doors.mat", "doors");

		sponza->setMaterial("floors.mat", "floors");
	}
	sponza->SetTransform(Mat4::translate(Vec3(0, 20, 0)));
	sponza->setPhysicsStaticMesh();

	for (int i = 0; i < 5; i++) {
		box[i] = new ObjectMesh("cube.nggf");
		box[i]->setMaterial("grid.mat");
		box[i]->SetTransform(Mat4::translate(Vec3(-10 - i * 2, i * 20 + 10, i - 10)));
		box[i]->setPhysicsBox(Vec3(10, 10, 10), 10);
		box[i]->setImpactSound("impact.ogg");
	}

	for (int i = 0; i < 5; i++) {
		sphere[i] = new ObjectMesh("sphere.nggf");
		sphere[i]->setMaterial("grid.mat");
		sphere[i]->SetTransform(Mat4::translate(Vec3(10 + i * 2, i * 20 + 10, i - 10)));
		sphere[i]->setPhysicsSphere(Vec3(5, 5, 5), 10);
		sphere[i]->setImpactSound("impact.ogg");
	}

	for (int i = 0; i < 5; i++) {
		cylinder[i] = new ObjectMesh("torus.nggf");
		cylinder[i]->setMaterial("grid.mat");
		cylinder[i]->SetTransform(Mat4::translate(Vec3(20 + i * 2, i * 20 + 20, i - 10)));
		cylinder[i]->setPhysicsConvexHull(10.0f);
		cylinder[i]->setImpactSound("impact.ogg");
	}

	camera = new CameraFree();
	camera->setPosition(Vec3(0, 20, 0));
	camera->setMaxVelocity(2000);
	camera->setPhysics(Vec3(5, 5, 5), 1.0f);
	camera->setFOV(60);

	omniLight = new LightPoint();
	omniLight->setColor(Vec3(1, 1, 0.8));
	omniLight->setPosition(Vec3(0, 60, 0));
	omniLight->setRadius(200);
	
	auto light = new LightSpot();
	light->setColor(Vec3(1, 1, 0.8));
	light->setPosition(Vec3(-10, -5, 60));

	auto lightD = new LightDirect();
	lightD->setColor(Vec3(1, 1, 0.8));
	lightD->setPosition(Vec3(0, 40, 0));
	lightD->setShadows(false);

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

	GetScene()->setAmbient(Vec3(0.2, 0.2, 0.2));
	sMesh = new ObjectSkinnedMesh("chammy.nggf");
	sMesh->setMaterial("grid.mat");
	sMesh->SetTransform(Mat4::translate(Vec3(-60, 60, 0)));

	GetWindow()->grabMouse(true);
#endif
}
//------------------------------------------------------------
EventsCallback::EventsCallback(){}
//------------------------------------------------------------

void ShowConsole()
{
	bool status = GetEngine()->ConsoleIsActive();
	GetEngine()->ConsoleShow(!status);
	GetWindow()->grabMouse(status);
}

void EventsCallback::Body(){
	if (GetWindow()->isKeyDown("ESC"))
		exit(0);

	if (GetWindow()->isKeyDown("GRAVE ACCENT"))
	{
		ShowConsole();
	}

	if (GetWindow()->isKeyDown("F12"))
		API_Make_ScreenShot();
}

#endif