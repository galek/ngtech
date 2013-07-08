#include "EnginePrivate.h"
#include "Src/Engine.h"
#include "Main.h"
#include "Src\Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_Gui.h"
#include "Src\Includes\MyGUI_3.2.0\MyGUIEngine\include\MyGUI_TextBox.h"
using namespace MyGUI;
using namespace NGEngine;

#ifdef SPONZA_DEMO
//objects
ObjectMesh *sponza;
ObjectMesh *box[5];
ObjectMesh *sphere[5];
ObjectMesh *cylinder[5];
ObjectSkinnedMesh *chammy;
CameraFree *camera;
LightOmni *omniLight;
MyGUI::TextBox *fpsLabel;

ParticleSystem *particlesPink;
ParticleSystem *particlesYellow;

//------------------------------------------------------------
void init() {
	//initializing loading screen
	LoadingScreen *lscreen = LoadingScreen::create("data/textures/logos/background.jpg");
	lscreen->show();


#if 0
	chammy = new ObjectSkinnedMesh("chammy.asmdl");
	chammy->setMaterial("*", "usor.amtr");
	chammy->setTransform(Mat4::translate(Vec3(10, 10, 10)));
#endif
	sponza = new ObjectMesh("sponza.amdl");
	sponza->setMaterialList("sponza.amtrlst");
	sponza->setPhysicsStaticMesh();
	sponza->setTransform(Mat4::translate(Vec3(0, -10, 0)));
#if 1
	for(int i = 0; i < 5; i++) {
	box[i] = new ObjectMesh("cube.amdl");
	box[i]->setMaterial("*", "grid.amtr");
	box[i]->setPhysicsBox(Vec3(10, 10, 10), 10);
	box[i]->setTransform(Mat4::translate(Vec3(-10-i*2, i*20 + 10, i - 10)));
	box[i]->setImpactSound("impact.ogg");
	}

	//for(int i = 0; i < 1; i++) {
	//sphere[i] = new ObjectMesh("sphere.amdl");
	//sphere[i]->setMaterial("*", "grid.amtr");
	//sphere[i]->setPhysicsSphere(Vec3(5,5,5), 10);
	//sphere[i]->setTransform(Mat4::translate(Vec3(10+i*2, i*20 + 10, i - 10)));
	//sphere[i]->setImpactSound("impact.ogg");

	//}
#if 0
	for(int i = 0; i < 5; i++) {
		cylinder[i] = new ObjectMesh("torus.amdl");
		cylinder[i]->setMaterial("*", "grid.amtr");
		//cylinder[i]->setPhysicsConvexHull(10);
		cylinder[i]->setTransform(Mat4::translate(Vec3(20+i*2, i*20 + 20, i - 10)));
		//cylinder[i]->setImpactSound("impact.ogg");


	}
#endif
#endif	

	fpsLabel = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>("TextBox", 100,100,180,180, MyGUI::Align::Default, "Statistic","InfoTextBox");
	fpsLabel->setTextColour(MyGUI::Colour::White);
	fpsLabel->setTextShadow(true);
	fpsLabel->setVisible(true);
	fpsLabel->setCaption("FPS: ");



	camera = new CameraFree();
	camera->setPosition(Vec3(0, 20, 0));
	camera->setMaxVelocity(2000);
	camera->setPhysics(Vec3(5, 5, 5), 1.0);
	camera->setFOV(60);


	omniLight = new LightOmni();
	omniLight->setColor(Vec3(1, 1, 1));
	omniLight->setPosition(Vec3(0, 60, 0));
	omniLight->setRadius(200);

	particlesPink = new ParticleSystem("data/textures/smoke.png", 50);
	particlesPink->setPosition(Vec3(60, 60, 0));
	particlesPink->setColor(Vec3(1, 0.5, 1));
	particlesPink->setForce(Vec3(-0.5, 1, -0.7));
	particlesPink->setVelocity(Vec3(-10, 0, 0));
	particlesPink->setParticleLifeTime(10000);
	particlesPink->setDispersion(0.1);

	particlesYellow = new ParticleSystem("data/textures/smoke.png", 50);
	particlesYellow->setPosition(Vec3(-60, 60, 0));
	particlesYellow->setColor(Vec3(1, 1, 0.7));
	particlesYellow->setForce(Vec3(0.5, 1, 0.5));
	particlesYellow->setVelocity(Vec3(10, 0, 0));
	particlesYellow->setParticleLifeTime(10000);
	particlesYellow->setDispersion(0.1);

	engine.scene->setGravity(Vec3(0, -9.8, 0));
	engine.scene->setWater(1, 400);
	engine.scene->setAmbient(Vec3(0.2, 0.2, 0.2));

	engine.iWindow->grabMouse(true);
}

//------------------------------------------------------------
void render() {

}

float depth = 10;
//------------------------------------------------------------
void events() {
	//chammy->setFrame((int)(engine.iWindow->getETime() * 0.05)%500);

	if(engine.iWindow->wasKeyPressed(WindowSystem::KEY_ESC)) 
		engine.iWindow->toggleGrabMouse();

	//
	//		for(int i = 0; i < 4; i++) {
	//			if(cb[i]->wasPressed()) {
	//				engine.config->setInt("light_shadowtype", i);
	//				engine.scene->reloadShaders();
	//			}
	//		}
	//
	//		if(cbSP->isPressed()) {
	//			engine.config->setBool("light_specular", cbSP->isChecked());
	//			engine.scene->reloadShaders();
	//		}
	//
	//		if(cbHDR->isPressed()) {
	//			engine.config->setBool("effect_hdr", cbHDR->isChecked());
	//		}
	//		
	//		if(cbWire->isPressed()) {
	//			engine.config->setBool("debug_wireframe", cbWire->isChecked());
	//		}
	//
	if(engine.iWindow->getDTime() > EPSILON)
		fpsLabel->setCaption("FPS: " + StringHelper::fromInt(1000/engine.iWindow->getDTime()));
	//
	//		if(exitButton->isPressed()) {
	////TODO:Nick			Engine::get()->quit();
	//		}
	//
	if(engine.iWindow->isKeyPressed(WindowSystem::KEY_Z)) {
		engine.scene->setWater(depth += 1, 500);
	}

	if(engine.iWindow->isKeyPressed(WindowSystem::KEY_X)) {
		engine.scene->setWater(depth -= 1, 500);
	}
}
#endif