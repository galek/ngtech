//***************************************************
#include "EnginePrivate.h"
//***************************************************
#include "../../Platform/inc/glfw/glfw3.h"
#include "../../Platform/inc/glfw/glfw3native.h"
#include "mygui.h"
//***************************************************
#include "CvarManager.h"
//***************************************************
#include "../inc/WindowSystem_GLFW.h"
#include "../inc/WindowEvents.h"
//***************************************************


namespace NGTech {

	/**
	*/
	WindowSystemGLFW::WindowSystemGLFW(CVARManager*_cvars)
		:cvars(_cvars),
		mouseGrabed(false),
		mousing(false),
		cursorVisible(false),
		mouseX(0),
		mouseY(0),
		oldMouseX(0),
		oldMouseY(0),
		width(_cvars->r_width),
		height(_cvars->r_height),
		bpp(_cvars->r_bpp),
		zdepth(_cvars->r_zdepth),
		fullscreen(_cvars->r_fullscreen)
	{
		Log::writeHeader("-- WindowSystem --");

		for (int i = 0; i < 3; i++)
			this->mouseButtons[i] = false;

		for (int i = 0; i < 315; i++)
			this->keys[i] = false;
	}

	/*
	*/
	void WindowSystemGLFW::initialise(int)
	{
		Debug("WindowSystem::Initialize()");

		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
		{
			glfwTerminate();
			Error("Failed initializing GLFW window{glfwInit}", true);
		}

		//Request Specific Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWmonitor *mMonitor = NULL;
		if (fullscreen)
			mMonitor = glfwGetPrimaryMonitor();
		window = glfwCreateWindow(width, height, "NG Engine", mMonitor, NULL);
		if (!window)
		{
			glfwTerminate();
			Error("Failed initializing GLFW window{glfwCreateWindow}", true);
		}

		showOSCursor(false);

		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetWindowCloseCallback(window, window_close_callback);

		if (!(hRC = glfwGetWGLContext(window)))
			Error("WindowSystem::Initialize() error: can't Create a GL rendering context", true);

		_updateFPSCounter();
	}

	/*
	*/
	WindowSystemGLFW::~WindowSystemGLFW() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	/*
	*/
	void WindowSystemGLFW::setTitle(const String &title) {
		glfwSetWindowTitle(window, title.c_str());
	}

	/*
	*/
	void WindowSystemGLFW::update() {
		_updateFPSCounter();
		//updateTimer();

		memcpy(oldKeys, keys, sizeof(keys));
		memcpy(oldMouseButtons, mouseButtons, sizeof(mouseButtons));

		glfwPollEvents();
	}

	/**
	*/
	bool WindowSystemGLFW::isKeyPressed(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return keys[key];
	}

	/**
	*/
	bool WindowSystemGLFW::isKeyDown(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return (keys[key] && !oldKeys[key]);
	}

	/**
	*/
	bool WindowSystemGLFW::isKeyUp(const char* _key) {
		const int key = Input_GetKeyValueByChar(_key);
		return (!keys[key] && oldKeys[key]);
	}

	/**
	*/
	bool WindowSystemGLFW::isMouseButtonPressed(int mb) {
		return mouseButtons[mb];
	}

	/**
	*/
	bool WindowSystemGLFW::wasMouseButtonPressed(int mb) {
		return (mouseButtons[mb] && !oldMouseButtons[mb]);
	}

	/**
	*/
	bool WindowSystemGLFW::wasMouseButtonReleased(int mb) {
		return (!mouseButtons[mb] && oldMouseButtons[mb]);
	}

	/**
	*/
	void WindowSystemGLFW::showCursor(bool show) {
		MyGUI::PointerManager::getInstance().setVisible(show);
		cursorVisible = show;
	}

	/**
	*/
	void WindowSystemGLFW::showOSCursor(int show){
		int value = GLFW_CURSOR_NORMAL;
		if (show == 1)
			value = GLFW_CURSOR_NORMAL;
		else if ((show == 2) || ((cvars->r_fullscreen == false) && (cvars->i_w_mousefree == true)))
			value = GLFW_CURSOR_HIDDEN;
		else
			value = GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, value);
	}

	/**
	*/
	void WindowSystemGLFW::setMousePos(int x, int y) {
	}

	/**
	*/
	void WindowSystemGLFW::grabMouse(bool grab) {
		showCursor(!grab);
		mouseGrabed = grab;
	}

	/**
	*/
	void WindowSystemGLFW::swapBuffers() {
		glfwSwapBuffers(window);
	}

	/**
	*/
	void WindowSystemGLFW::updateTimer() {
	}

	/**
	*/
	void WindowSystemGLFW::_updateFPSCounter(float theTimeInterval)
	{
		// Static values which only get initialised the first time the function runs
		static double t0Value = glfwGetTime(); // Set the initial time to now
		static int fpsFrameCount = 0;             // Set the initial FPS frame count to 0

		// Get the current time in seconds since the program started (non-static, so executed every time)
		double currentTime = glfwGetTime();

		// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
		// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
		if (theTimeInterval < 0.1f)
			theTimeInterval = 0.1f;
		if (theTimeInterval > 10.0f)
			theTimeInterval = 10.0f;

		dTime = (currentTime - t0Value);
		// Calculate and display the FPS every specified time interval
		if ((currentTime - t0Value) > theTimeInterval)
		{
			// Calculate the FPS as the number of frames divided by the interval in seconds
			fps = (double)fpsFrameCount / dTime;

			// Reset the FPS frame counter and set the initial time to be now
			fpsFrameCount = 0;
			t0Value = glfwGetTime();
		}
		else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
			fpsFrameCount++;
	}

	/**
	*/
	int WindowSystemGLFW::getTime() {
		//TODO
		return 0;
	}

	/**
	*/
	const int WindowSystemGLFW::Input_GetKeyValueByChar(const char*  _p){
		return _get_key_name(_p);
	}

	/**
	*/
	const char* WindowSystemGLFW::Input_GetKeyValueByInt(int _p){
		return get_key_name(_p);
	}
}