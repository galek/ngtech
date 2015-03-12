#pragma once

//***************************************************
#include "../../Platform/inc/glfw/glfw3.h"
//***************************************************

namespace NGTech {

	/**
	Engine`s main window and input system. Created one time
	*/
	class ENGINE_API WindowSystemGLFW :public I_Window {
	public:
		/**
		Mouse buttons enum
		*/
		enum MouseButton {
			MOUSE_LEFT_BUTTON = GLFW_MOUSE_BUTTON_LEFT,
			MOUSE_RIGHT_BUTTON = GLFW_MOUSE_BUTTON_RIGHT,
			MOUSE_RIGHT_MIDLLE = GLFW_MOUSE_BUTTON_MIDDLE
		};

		/**
		Keyboard keys enum
		*/
		enum Key {
			KEY_ESC = GLFW_KEY_ESCAPE,
			KEY_SPACE = GLFW_KEY_SPACE,
			KEY_MINUS = GLFW_KEY_MINUS,
			KEY_RETURN = GLFW_KEY_BACKSPACE,
			KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
			KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
			KEY_EQUAL = GLFW_KEY_EQUAL,
			KEY_BACKSLASH = GLFW_KEY_BACKSLASH,
			KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
			KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
			KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,

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

			KEY_F1 = GLFW_KEY_F1, KEY_F2,
			KEY_F3, KEY_F4,
			KEY_F5, KEY_F6,
			KEY_F7, KEY_F8,
			KEY_F9, KEY_F10,
			KEY_F11, KEY_F12,
			KEY_F13, KEY_F14,
			KEY_F15, KEY_F16,
			KEY_F17, KEY_F18,
			KEY_F19, KEY_F20,
			KEY_F21, KEY_F22,
			KEY_F23, KEY_F24,
			KEY_F25
		};
	public:
		/**
		*/
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
		ENGINE_INLINE virtual int getWidth() { return width; }

		/**
		Gets screen height
		\return screen height
		*/
		ENGINE_INLINE virtual int getHeight() { return height; }

		/**
		Gets frames delta time
		\return dTime
		*/
		ENGINE_INLINE virtual int getDTime() { return dTime; }

		/**
		Gets frames ticks
		\return ulTicks
		*/
		virtual unsigned long GetTicks();

		/**
		Gets elapsed time
		\return eTime
		*/
		ENGINE_INLINE virtual int getETime() { return eTime; }//TODO

		/**
		Pauses timer
		\param pause pause flag
		*/
		ENGINE_INLINE virtual void timerPause(bool pause) { tPause = pause; }

		/**
		Get time in mseconds
		\return time in mseconds
		*/
		virtual int getTime();

		/**
		Checks if mouse was moved
		\return true if moved
		*/
		ENGINE_INLINE virtual bool isMouseMoved() { return mousing; }

		/**
		Gets mouse X coordinate
		\return X coordinate
		*/
		ENGINE_INLINE virtual int getMouseX() { return mouseX; }

		/**
		Gets mouse Y coordinate
		\return Y coordinate
		*/
		ENGINE_INLINE virtual int getMouseY() { return mouseY; }

		/**
		Gets mouse delta X coordinate
		\return delta X coordinate
		*/
		ENGINE_INLINE virtual int getMouseDX() { return mouseX - oldMouseX; }

		/**
		Gets mouse delta Y coordinate
		\return delta Y coordinate
		*/
		ENGINE_INLINE int getMouseDY() { return mouseY - oldMouseY; }

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
		ENGINE_INLINE void toggleShowCursor() { showCursor(!cursorVisible); }

		/**
		Checks if cursor is visible
		\return true if visible
		*/
		ENGINE_INLINE bool isCursorVisible() { return cursorVisible; }

		/**
		Grab/release cursor
		\param grab grab cursor if true
		*/
		void grabMouse(bool grab);

		/**
		Toggle grab/release cursor
		*/
		ENGINE_INLINE void toggleGrabMouse() { grabMouse(!mouseGrabed); }

		/**
		Checks if cursor is grabed
		\return true if grabed
		*/
		ENGINE_INLINE bool isMouseGrabed() { return mouseGrabed; }

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
		ENGINE_INLINE virtual void setKeyDown(int _value){
			keys[_value] = true;
		}

		/**
		*/
		ENGINE_INLINE virtual void setKeyUp(int _value){
			keys[_value] = false;
		}

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
		virtual void showOSCursor(int _value);
		/**
		*/
		virtual void ManageVSync(bool _v);
		/**
		*/
		ENGINE_INLINE virtual int divider() { return 1000; }
	private:
		/**
		*/
		void _updateFPSCounter(float theTimeInterval = 1.0f);
		/**
		*/
		const int _get_key_name(const char* _p);
	public:
		int width, height, bpp, zdepth;
		bool fullscreen, withoutBorder;

		//Delme
		int eTime, dTime;
		bool tPause;
		//.end

		bool mouseButtons[3];
		bool oldMouseButtons[3];

		bool keys[GLFW_KEY_LAST];
		bool oldKeys[GLFW_KEY_LAST];

		int mouseX, mouseY, oldMouseX, oldMouseY;
		bool mousing, cursorVisible, mouseGrabed;
	private:
		float fps;
		unsigned long ulTicks;
	private:
		GLFWwindow* window;
		CVARManager* cvars;
	};
}