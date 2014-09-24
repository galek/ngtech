#pragma once

//***************************************************
#include "../../Platform/inc/glfw/glfw3.h"
//***************************************************

namespace NGTech {

	/**
	Engine`s main window and input system. Created one time
	*/
	class ENGINE_API WindowSystemGLFW :public IWindow {
	public:
		/**
		Mouse buttons enum
		*/
		enum MouseButton {
			MOUSE_LEFT_BUTTON = GLFW_MOUSE_BUTTON_LEFT,
			MOUSE_RIGHT_BUTTON = GLFW_MOUSE_BUTTON_RIGHT
		};

		/**
		Keyboard keys enum
		*/
		enum Key {
			KEY_ESC = GLFW_KEY_ESCAPE,
			KEY_SPACE = GLFW_KEY_SPACE,
			KEY_RETURN = GLFW_KEY_BACKSPACE,

			KEY_UP = GLFW_KEY_UP,
			KEY_DOWN = GLFW_KEY_DOWN,
			KEY_LEFT = GLFW_KEY_LEFT,
			KEY_RIGHT = GLFW_KEY_RIGHT,

			KEY_0 = GLFW_KEY_KP_0,
			KEY_1,
			KEY_2, KEY_3,
			KEY_4, KEY_5,
			KEY_6, KEY_7,
			KEY_8, KEY_9,

			KEY_A = 65,
			KEY_B,
			KEY_C, KEY_D,
			KEY_E, KEY_F,
			KEY_G, KEY_H,
			KEY_I, KEY_J,
			KEY_K, KEY_L,
			KEY_M, KEY_N,
			KEY_O, KEY_P,
			KEY_Q, KEY_R,
			KEY_S, KEY_T,
			KEY_U, KEY_V,
			KEY_W, KEY_X,
			KEY_Y, KEY_Z,
		};

	public:
		explicit WindowSystemGLFW(CVARManager*_cvars);
		/**
		Creates new WindowSystem
		\param width screen width
		\param height screen height
		\param bpp screen bpp
		\param zDepth ZBuffer depth
		\param fullscr fullscreen flag
		\param windowTitle window title string
		*/
		virtual void initialise(int);


		/**
		Destroys WindowSystem
		*/
		virtual ~WindowSystemGLFW();

		/**
		Sets window title
		\param title title text
		*/
		virtual void setTitle(const String &title);

		/**
		Updates app`s timer
		*/
		virtual void updateTimer();

		/**
		Swaps app`s back and front buffers
		*/
		virtual void swapBuffers();

		/**
		Updates WindowSystem and processes events
		*/
		virtual void update();

		/**
		Gets screen width
		\return screen width
		*/
		virtual int getWidth() { return width; }

		/**
		Gets screen height
		\return screen height
		*/
		virtual int getHeight() { return height; }

		/**
		Gets frames delta time
		\return dTime
		*/
		virtual int getDTime() { return dTime; }

		/**
		Gets elapsed time
		\return eTime
		*/
		virtual int getETime() { return eTime; }

		/**
		Pauses timer
		\param pause pause flag
		*/
		virtual void timerPause(bool pause) { tPause = pause; }

		/**
		Get time in mseconds
		\return time in mseconds
		*/
		virtual int getTime();

		/**
		Checks if mouse was moved
		\return true if moved
		*/
		virtual bool isMouseMoved() { return /*mousing*/false; }

		/**
		Gets mouse X coordinate
		\return X coordinate
		*/
		virtual int getMouseX() { return /*mouseX*/0; }

		/**
		Gets mouse Y coordinate
		\return Y coordinate
		*/
		virtual int getMouseY() { return /*mouseY*/0; }

		/**
		Gets mouse delta X coordinate
		\return delta X coordinate
		*/
		virtual int getMouseDX() { return /*mouseX - oldMouseX*/0; }

		/**
		Gets mouse delta Y coordinate
		\return delta Y coordinate
		*/
		int getMouseDY() { return/* mouseY - oldMouseY*/0; }

		/**
		Sets mouse position
		\param x mouse x
		\param y mouse y
		*/
		void setMousePos(int x, int y);

		/**
		Show/hide cursor
		\param show show cursor if true
		*/
		void showCursor(bool show);

		/**
		Toggle show/hide cursor
		*/
		void toggleShowCursor() { /*showCursor(!cursorVisible);*/ }

		/**
		Checks if cursor is visible
		\return true if visible
		*/
		bool isCursorVisible() { return /*cursorVisible*/false; }

		/**
		Grab/release cursor
		\param grab grab cursor if true
		*/
		void grabMouse(bool grab);

		/**
		Toggle grab/release cursor
		*/
		void toggleGrabMouse() { /*grabMouse(!mouseGrabed);*/ }

		/**
		Checks if cursor is grabed
		\return true if grabed
		*/
		bool isMouseGrabed() { return /*mouseGrabed*/false; }

		/**
		Checks if the mouse button is pressed
		\param mb mouse button id
		\return true if pressed
		*/
		virtual bool isMouseButtonPressed(int mb);

		/**
		Checks if the mouse button was pressed in previous frame and now it is released
		\param mb mouse button id
		\return true if was pressed
		*/
		virtual bool wasMouseButtonPressed(int mb);

		/**
		Checks if the mouse button was released in previous frame and now it is pressed
		\param mb mouse button id
		\return true if was pressed
		*/
		virtual bool wasMouseButtonReleased(int mb);

		/**
		Checks if the key is pressed
		\param key key id
		\return true if pressed
		*/
		virtual bool isKeyPressed(const char*  key);

		/**
		Checks if the key was pressed in previous frame and now it is released
		\param key key id
		\return true if was pressed
		*/
		virtual bool isKeyDown(const char*  key);

		/**
		Checks if the key was released in previous frame and now it is pressed
		\param key key id
		\return true if was released
		*/
		virtual bool isKeyUp(const char*  key);
		
		/**
		*/
		virtual const int Input_GetKeyValueByChar(const char*  _p);
		/**
		*/
		virtual const char* Input_GetKeyValueByInt(int _p);
		/**
		*/
		ENGINE_INLINE virtual float getLastFPS() { return fps; }
		/**
		*/
		virtual void showOSCursor(bool _value);
	private:
		/**
		*/
		void _updateFPSCounter(float theTimeInterval = 1.0f);
		/**
		*/
		const int _get_key_name(const char* _p);
	public:
		HGLRC hRC;
		HINSTANCE hInstance;
	public:
		int width, height, bpp, zdepth;
		bool fullscreen;

		int eTime, dTime;
		bool tPause;

		bool mouseButtons[3];
		bool oldMouseButtons[3];

		bool keys[315];
		bool oldKeys[315];
	private:
		float fps;
	private:
		GLFWwindow* window;
	};
}