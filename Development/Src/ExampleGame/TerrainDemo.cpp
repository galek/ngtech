#include "stdafx.h"

using namespace VEGA;

#ifdef TERRAIN_DEMO
ObjectMesh *box[5];
ObjectMesh *sphere[5];
ObjectMesh *cylinder[5];
ObjectMesh *airplane;

ObjectSkinnedMesh *chammy;
CameraFree *camera;
ParticleSystem *particles;
LightDirect *directLight;

//GUI widgets
WidgetWindow *window;
WidgetLabel *label;
WidgetCheckBox *cb[4], *cbSP, *cbHDR, *cbWire;
WidgetRadioGroup *group;
WidgetButton *exitButton;
WidgetLabel *fpsLabel;

//------------------------------------------------------------
void init() {
	//initializing loading screen
	LoadingScreen *lscreen = LoadingScreen::create("../data/textures/logos/background.jpg");
	lscreen->show();

	//initialize GUI
	GUI::create("../data/textures/gui");
	GUI::get()->setAlpha(0.8);

	GUI::get()->addWidget(fpsLabel = new WidgetLabel("FPS: "));
	fpsLabel->setPosition(20, 40);

	GUI::get()->addWidget(window = new WidgetWindow());
	window->setPosition(500, 300);
	window->setActive(false);

	window->addWidget(label = new WidgetLabel("Settings"));
	label->setPosition(10, 10);
	window->addWidget(group = new WidgetRadioGroup());
	group->setPosition(10, 10);

	window->addWidget(cbSP = new WidgetCheckBox("specular"));
	cbSP->setPosition(10, 180);
	cbSP->setChecked(GetEngine()->config->getInt("light_specular"));

	window->addWidget(cbHDR = new WidgetCheckBox("HDR bloom"));
	cbHDR->setChecked(GetEngine()->config->getBool("effect_hdr"));
	cbHDR->setPosition(10, 210);

	window->addWidget(cbWire = new WidgetCheckBox("wireframe"));
	cbWire->setChecked(GetEngine()->config->getBool("debug_wireframe"));
	cbWire->setPosition(10, 240);

	group->addCheckBox(cb[0] = new WidgetCheckBox("no shadows"));
	cb[0]->setPosition(10, 30);
	group->addCheckBox(cb[1] = new WidgetCheckBox("sm shadows"));
	cb[1]->setPosition(10, 60);
	group->addCheckBox(cb[2] = new WidgetCheckBox("sm shadows with PCF 2x2"));
	cb[2]->setPosition(10, 90);
	group->addCheckBox(cb[3] = new WidgetCheckBox("sm shadows with PCF 3x3"));
	cb[3]->setPosition(10, 120);
	cb[GetEngine()->config->getInt("light_shadowtype")]->setChecked(true);

	window->addWidget(exitButton = new WidgetButton("Exit"));
	exitButton->setPosition(300, 250);

	//SCENE
	Scene::create();

	chammy = new ObjectSkinnedMesh("chammy.asmdl");
	chammy->setMaterial("*", "usor.amtr");
	chammy->setTransform(Mat4::translate(Vec3(30, 120, 0)));
	//¹(chammy);

	airplane = new ObjectMesh("airplane.amdl");
	airplane->setMaterial("box01", "box01.amtr");
	airplane->setMaterial("object02", "object02.amtr");
	airplane->setMaterial("object03", "object03.amtr");
	airplane->setTransform(Mat4::translate(Vec3(0, 120, 0)));
	¹(airplane);


	for(int i = 0; i < 5; i++) {
		box[i] = new ObjectMesh("cube.amdl");
		box[i]->setMaterial("*", "grid.amtr");
		box[i]->setPhysicsBox(Vec3(10, 10, 10), 10);
		box[i]->setTransform(Mat4::translate(Vec3(-80-i*2, i*20 + 50, i + 20)));
		box[i]->setImpactSound("impact.ogg");
		¹(box[i]);
	}

	for(int i = 0; i < 5; i++) {
		sphere[i] = new ObjectMesh("sphere.amdl");
		sphere[i]->setMaterial("*", "grid.amtr");
		sphere[i]->setPhysicsSphere(Vec3(5, 5, 5), 10);
		sphere[i]->setTransform(Mat4::translate(Vec3(-90-i*2, i*20 + 50, i + 20)));
		sphere[i]->setImpactSound("impact.ogg");
		¹(sphere[i]);
	}

	for(int i = 0; i < 5; i++) {
		cylinder[i] = new ObjectMesh("torus.amdl");
		cylinder[i]->setMaterial("*", "grid.amtr");
		cylinder[i]->setPhysicsConvexHull(10);
		cylinder[i]->setTransform(Mat4::translate(Vec3(-100-i*2, i*20 + 50, i + 20)));
		cylinder[i]->setImpactSound("impact.ogg");
		¹(cylinder[i]);
	}

	camera = new CameraFree();
	camera->setPosition(Vec3(0, 100, 0));
	camera->setMaxVelocity(20000);
	camera->setPhysics(Vec3(5, 5, 5), 1.0);
	GetEngine()->scene->setCamera(camera);

	LightOmni *omniLight = new LightOmni();
	omniLight->setColor(Vec3(1, 1, 1));
	omniLight->setPosition(Vec3(0, 250, 0));
	omniLight->setRadius(400);
	GetEngine()->scene->addLight(omniLight);

	directLight = new LightDirect();
	directLight->setDirection(Vec3(0.5, -0.8, 0.5));
	directLight->setColor(Vec3(1.0, 1.0, 0.8));
	GetEngine()->scene->addLight(directLight);

	GetEngine()->scene->setGravity(Vec3(0, -9.8, 0));
	GetEngine()->scene->setWater(55, 1000);
	GetEngine()->scene->setTerrain("../data/textures/HeightMap_small.tga", 1.5, 100.0, true);
	GetEngine()->scene->setTerrainLods(Vec3(400, 600, 900));
	GetEngine()->scene->setTerrainMaterial("terrain.amtr");
	GetEngine()->scene->setAmbient(Vec3(0.2, 0.2, 0.2));

	GetEngine()->iWindow->grabMouse(true);

	GetEngine()->iRender->clearColor(Vec3(0.5, 0.5, 1.0));
}

//------------------------------------------------------------
void render() {
}

float depth = 20;
//------------------------------------------------------------
void events() {
	chammy->setFrame((int)(GetEngine()->iWindow->getETime() * 0.01)%100);
	if(GetEngine()->iWindow->wasKeyPressed(WindowSystem::KEY_ESC)) {
		window->toggleActive();
		GetEngine()->iWindow->toggleGrabMouse();
	}

	for(int i = 0; i < 4; i++) {
		if(cb[i]->wasPressed()) {
			GetEngine()->config->setInt("light_shadowtype", i);
			GetEngine()->scene->reloadShaders();
		}
	}

	if(cbSP->wasPressed()) {
		GetEngine()->config->setBool("light_specular", cbSP->isChecked());
		GetEngine()->scene->reloadShaders();
	}

	GetEngine()->config->setBool("system_hdr", cbHDR->isChecked());
	GetEngine()->config->setBool("debug_wireframe", cbWire->isChecked());

	if(GetEngine()->iWindow->getDTime() > EPSILON)
		fpsLabel->setText("FPS: " + StringHelper::fromInt(1000/GetEngine()->iWindow->getDTime()));

	if(exitButton->isPressed()) {
		Engine::get()->quit();
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_Z)) {
		GetEngine()->scene->setWater(depth += 1, 500);
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_X)) {
		GetEngine()->scene->setWater(depth -= 1, 500);
	}
}
#endif