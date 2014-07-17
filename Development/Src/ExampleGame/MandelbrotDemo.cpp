#include "EnginePrivate.h"

//using namespace NGTech;

#ifdef MANDELBROT_DEMO
GLShader *mandelbotShader;
GLTexture *usor;
WidgetLabel *scaleLabel, *positionLabel;
WidgetButton *exitButton;

Vec2 position, scale;

void init() {
	GUI::create("../data/textures/gui");

	mandelbotShader = GLShader::create("../data/shaders/mandelbrot.ashd");
	usor = GetRender()->TextureCreate2D("../data/textures/usor.jpg");
	usor->setFilter(I_Texture::LINEAR);

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
	GetRender()->enable2d(true);

	mandelbotShader->set();
	mandelbotShader->sendVec2("position", position);
	mandelbotShader->sendVec2("scale", scale);
	mandelbotShader->sendInt("colorMap", 0);

	usor->set(0);
	GetRender()->drawRect(0, 0, 1, 1, 0, 1, 1, 0);
	usor->unset(0);

	mandelbotShader->unset();
}

void events() {
	if(GetWindow()->isMouseButtonPressed(WindowSystem::MOUSE_LEFT_BUTTON)) {
		float s = (float)GetWindow()->getDTime() / 1000.0;
		scale = scale * 1.0 / (1.0 + s * 0.5);
	}

	if(GetWindow()->isMouseButtonPressed(WindowSystem::MOUSE_RIGHT_BUTTON)) {
		float s = (float)GetWindow()->getDTime() / 1000.0;
		scale = scale * (1.0 + s * 0.5);
	}

	if(GetWindow()->isKeyPressed(WindowSystem::KEY_UP)) {
		float p = (float)GetWindow()->getDTime() / 1000.0 * 0.5;
		position.y += p * scale.y;
	}

	if(GetWindow()->isKeyPressed(WindowSystem::KEY_DOWN)) {
		float p = (float)GetWindow()->getDTime() / 1000.0 * 0.5;
		position.y -= p * scale.y;
	}

	if(GetWindow()->isKeyPressed(WindowSystem::KEY_LEFT)) {
		float p = (float)GetWindow()->getDTime() / 1000.0 * 0.5;
		position.x -= p * scale.x;
	}

	if(GetWindow()->isKeyPressed(WindowSystem::KEY_RIGHT)) {
		float p = (float)GetWindow()->getDTime() / 1000.0 * 0.5;
		position.x += p * scale.x;
	}

	if(exitButton->isPressed() || GetWindow()->isKeyDown(WindowSystem::KEY_ESC)) {
		Engine::get()->quit();
	}
}
#endif