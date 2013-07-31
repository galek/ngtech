#include "EnginePrivate.h"

//using namespace VEGA;

#ifdef MANDELBROT_DEMO
GLShader *mandelbotShader;
GLTexture *usor;
WidgetLabel *scaleLabel, *positionLabel;
WidgetButton *exitButton;

Vec2 position, scale;

void init() {
	GUI::create("../data/textures/gui");

	mandelbotShader = GLShader::create("../data/shaders/mandelbrot.ashd");
	usor = GLTexture::create2d("../data/textures/usor.jpg");
	usor->setFilter(GLTexture::LINEAR);

	GUI::get()->addWidget(scaleLabel = new WidgetLabel("Scale (mouse buttons)"));
	scaleLabel->setPosition(0, 0);

	GUI::get()->addWidget(positionLabel = new WidgetLabel("Position (arrow keys)"));
	positionLabel->setPosition(0, 15);

	GUI::get()->addWidget(exitButton = new WidgetButton("Exit"));
	exitButton->setPosition(20, 40);

	position = Vec2(-0.5, 0.0);
	scale    = Vec2(1.0, 1.0);
}

void render() {
	GetEngine()->iRender->enable2d(true);

	mandelbotShader->set();
	mandelbotShader->sendVec2("position", position);
	mandelbotShader->sendVec2("scale", scale);
	mandelbotShader->sendInt("colorMap", 0);

	usor->set(0);
	GetEngine()->iRender->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
	usor->unset(0);

	mandelbotShader->unset();
}

void events() {
	if(GetEngine()->iWindow->isMouseButtonPressed(WindowSystem::MOUSE_LEFT_BUTTON)) {
		float s = (float)GetEngine()->iWindow->getDTime() / 1000.0;
		scale = scale * 1.0 / (1.0 + s * 0.5);
	}

	if(GetEngine()->iWindow->isMouseButtonPressed(WindowSystem::MOUSE_RIGHT_BUTTON)) {
		float s = (float)GetEngine()->iWindow->getDTime() / 1000.0;
		scale = scale * (1.0 + s * 0.5);
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_UP)) {
		float p = (float)GetEngine()->iWindow->getDTime() / 1000.0 * 0.5;
		position.y += p * scale.y;
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_DOWN)) {
		float p = (float)GetEngine()->iWindow->getDTime() / 1000.0 * 0.5;
		position.y -= p * scale.y;
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_LEFT)) {
		float p = (float)GetEngine()->iWindow->getDTime() / 1000.0 * 0.5;
		position.x -= p * scale.x;
	}

	if(GetEngine()->iWindow->isKeyPressed(WindowSystem::KEY_RIGHT)) {
		float p = (float)GetEngine()->iWindow->getDTime() / 1000.0 * 0.5;
		position.x += p * scale.x;
	}

	if(exitButton->isPressed() || GetEngine()->iWindow->wasKeyPressed(WindowSystem::KEY_ESC)) {
		Engine::get()->quit();
	}
}
#endif