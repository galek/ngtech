//***************************************************
#include "EnginePrivate.h"
//***************************************************
#include <stdlib.h>
#include <iostream>
//***************************************************
#include "../../Platform/inc/glfw/glfw3.h"
#include "mygui.h"
//***************************************************
#include "../Core/inc/CVARManager.h"
#include "../inc/WindowSystem_GLFW.h"
//***************************************************

namespace NGTech {

	/**
	*/
	void error_callback(int error, const char* description)	{
		Error(std::string("[Window System]") + description, true);
	}

	/**
	*/
	const char* get_key_name(int key)
	{
		switch (key)
		{
			// Printable keys
		case GLFW_KEY_A:            return "A";
		case GLFW_KEY_B:            return "B";
		case GLFW_KEY_C:            return "C";
		case GLFW_KEY_D:            return "D";
		case GLFW_KEY_E:            return "E";
		case GLFW_KEY_F:            return "F";
		case GLFW_KEY_G:            return "G";
		case GLFW_KEY_H:            return "H";
		case GLFW_KEY_I:            return "I";
		case GLFW_KEY_J:            return "J";
		case GLFW_KEY_K:            return "K";
		case GLFW_KEY_L:            return "L";
		case GLFW_KEY_M:            return "M";
		case GLFW_KEY_N:            return "N";
		case GLFW_KEY_O:            return "O";
		case GLFW_KEY_P:            return "P";
		case GLFW_KEY_Q:            return "Q";
		case GLFW_KEY_R:            return "R";
		case GLFW_KEY_S:            return "S";
		case GLFW_KEY_T:            return "T";
		case GLFW_KEY_U:            return "U";
		case GLFW_KEY_V:            return "V";
		case GLFW_KEY_W:            return "W";
		case GLFW_KEY_X:            return "X";
		case GLFW_KEY_Y:            return "Y";
		case GLFW_KEY_Z:            return "Z";
		case GLFW_KEY_1:            return "1";
		case GLFW_KEY_2:            return "2";
		case GLFW_KEY_3:            return "3";
		case GLFW_KEY_4:            return "4";
		case GLFW_KEY_5:            return "5";
		case GLFW_KEY_6:            return "6";
		case GLFW_KEY_7:            return "7";
		case GLFW_KEY_8:            return "8";
		case GLFW_KEY_9:            return "9";
		case GLFW_KEY_0:            return "0";
		case GLFW_KEY_SPACE:        return "SPACE";
		case GLFW_KEY_MINUS:        return "MINUS";
		case GLFW_KEY_EQUAL:        return "EQUAL";
		case GLFW_KEY_LEFT_BRACKET: return "LEFT BRACKET";
		case GLFW_KEY_RIGHT_BRACKET: return "RIGHT BRACKET";
		case GLFW_KEY_BACKSLASH:    return "BACKSLASH";
		case GLFW_KEY_SEMICOLON:    return "SEMICOLON";
		case GLFW_KEY_APOSTROPHE:   return "APOSTROPHE";
		case GLFW_KEY_GRAVE_ACCENT: return "GRAVE ACCENT";
		case GLFW_KEY_COMMA:        return "COMMA";
		case GLFW_KEY_PERIOD:       return "PERIOD";
		case GLFW_KEY_SLASH:        return "SLASH";
		case GLFW_KEY_WORLD_1:      return "WORLD 1";
		case GLFW_KEY_WORLD_2:      return "WORLD 2";

			// Function keys
		case GLFW_KEY_ESCAPE:       return "ESCAPE";
		case GLFW_KEY_F1:           return "F1";
		case GLFW_KEY_F2:           return "F2";
		case GLFW_KEY_F3:           return "F3";
		case GLFW_KEY_F4:           return "F4";
		case GLFW_KEY_F5:           return "F5";
		case GLFW_KEY_F6:           return "F6";
		case GLFW_KEY_F7:           return "F7";
		case GLFW_KEY_F8:           return "F8";
		case GLFW_KEY_F9:           return "F9";
		case GLFW_KEY_F10:          return "F10";
		case GLFW_KEY_F11:          return "F11";
		case GLFW_KEY_F12:          return "F12";
		case GLFW_KEY_F13:          return "F13";
		case GLFW_KEY_F14:          return "F14";
		case GLFW_KEY_F15:          return "F15";
		case GLFW_KEY_F16:          return "F16";
		case GLFW_KEY_F17:          return "F17";
		case GLFW_KEY_F18:          return "F18";
		case GLFW_KEY_F19:          return "F19";
		case GLFW_KEY_F20:          return "F20";
		case GLFW_KEY_F21:          return "F21";
		case GLFW_KEY_F22:          return "F22";
		case GLFW_KEY_F23:          return "F23";
		case GLFW_KEY_F24:          return "F24";
		case GLFW_KEY_F25:          return "F25";
		case GLFW_KEY_UP:           return "UP";
		case GLFW_KEY_DOWN:         return "DOWN";
		case GLFW_KEY_LEFT:         return "LEFT";
		case GLFW_KEY_RIGHT:        return "RIGHT";
		case GLFW_KEY_LEFT_SHIFT:   return "LEFT SHIFT";
		case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT SHIFT";
		case GLFW_KEY_LEFT_CONTROL: return "LEFT CONTROL";
		case GLFW_KEY_RIGHT_CONTROL: return "RIGHT CONTROL";
		case GLFW_KEY_LEFT_ALT:     return "LEFT ALT";
		case GLFW_KEY_RIGHT_ALT:    return "RIGHT ALT";
		case GLFW_KEY_TAB:          return "TAB";
		case GLFW_KEY_ENTER:        return "ENTER";
		case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
		case GLFW_KEY_INSERT:       return "INSERT";
		case GLFW_KEY_DELETE:       return "DELETE";
		case GLFW_KEY_PAGE_UP:      return "PAGE UP";
		case GLFW_KEY_PAGE_DOWN:    return "PAGE DOWN";
		case GLFW_KEY_HOME:         return "HOME";
		case GLFW_KEY_END:          return "END";
		case GLFW_KEY_KP_0:         return "KEYPAD 0";
		case GLFW_KEY_KP_1:         return "KEYPAD 1";
		case GLFW_KEY_KP_2:         return "KEYPAD 2";
		case GLFW_KEY_KP_3:         return "KEYPAD 3";
		case GLFW_KEY_KP_4:         return "KEYPAD 4";
		case GLFW_KEY_KP_5:         return "KEYPAD 5";
		case GLFW_KEY_KP_6:         return "KEYPAD 6";
		case GLFW_KEY_KP_7:         return "KEYPAD 7";
		case GLFW_KEY_KP_8:         return "KEYPAD 8";
		case GLFW_KEY_KP_9:         return "KEYPAD 9";
		case GLFW_KEY_KP_DIVIDE:    return "KEYPAD DIVIDE";
		case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD MULTPLY";
		case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD SUBTRACT";
		case GLFW_KEY_KP_ADD:       return "KEYPAD ADD";
		case GLFW_KEY_KP_DECIMAL:   return "KEYPAD DECIMAL";
		case GLFW_KEY_KP_EQUAL:     return "KEYPAD EQUAL";
		case GLFW_KEY_KP_ENTER:     return "KEYPAD ENTER";
		case GLFW_KEY_PRINT_SCREEN: return "PRINT SCREEN";
		case GLFW_KEY_NUM_LOCK:     return "NUM LOCK";
		case GLFW_KEY_CAPS_LOCK:    return "CAPS LOCK";
		case GLFW_KEY_SCROLL_LOCK:  return "SCROLL LOCK";
		case GLFW_KEY_PAUSE:        return "PAUSE";
		case GLFW_KEY_LEFT_SUPER:   return "LEFT SUPER";
		case GLFW_KEY_RIGHT_SUPER:  return "RIGHT SUPER";
		case GLFW_KEY_MENU:         return "MENU";
		case GLFW_KEY_UNKNOWN:      return "UNKNOWN";

		default:                    return NULL;
		}
	}

	/**
	*/
	const int WindowSystemGLFW::_get_key_name(const char* _p)
	{
		if (stricmp(_p, "A") == 0)
			return KEY_A;
		else if (stricmp(_p, "B") == 0)
			return KEY_B;
		else if (stricmp(_p, "C") == 0)
			return KEY_C;
		else if (stricmp(_p, "D") == 0)
			return KEY_D;
		else if (stricmp(_p, "E") == 0)
			return KEY_E;
		else if (stricmp(_p, "F") == 0)
			return KEY_F;
		else if (stricmp(_p, "G") == 0)
			return KEY_G;
		else if (stricmp(_p, "H") == 0)
			return KEY_H;
		else if (stricmp(_p, "I") == 0)
			return KEY_I;
		else if (stricmp(_p, "J") == 0)
			return KEY_J;
		else if (stricmp(_p, "K") == 0)
			return KEY_K;
		else if (stricmp(_p, "L") == 0)
			return KEY_L;
		else if (stricmp(_p, "M") == 0)
			return KEY_M;
		else if (stricmp(_p, "N") == 0)
			return KEY_N;
		else if (stricmp(_p, "O") == 0)
			return KEY_O;
		else if (stricmp(_p, "P") == 0)
			return KEY_P;
		else if (stricmp(_p, "Q") == 0)
			return KEY_Q;
		else if (stricmp(_p, "R") == 0)
			return KEY_R;
		else if (stricmp(_p, "S") == 0)
			return KEY_S;
		else if (stricmp(_p, "T") == 0)
			return KEY_T;
		else if (stricmp(_p, "U") == 0)
			return KEY_U;
		else if (stricmp(_p, "V") == 0)
			return KEY_V;
		else if (stricmp(_p, "W") == 0)
			return KEY_W;
		else if (stricmp(_p, "X") == 0)
			return KEY_X;
		else if (stricmp(_p, "Y") == 0)
			return KEY_Y;
		else if (stricmp(_p, "Z") == 0)
			return KEY_Z;

		else if (_p[0] == '0')
			return KEY_0;
		else if (_p[0] == '1')
			return KEY_1;
		else if (_p[0] == '2')
			return KEY_2;
		else if (_p[0] == '3')
			return KEY_3;
		else if (_p[0] == '4')
			return KEY_4;
		else if (_p[0] == '5')
			return KEY_5;
		else if (_p[0] == '6')
			return KEY_6;
		else if (_p[0] == '7')
			return KEY_7;
		else if (_p[0] == '8')
			return KEY_8;
		else if (_p[0] == '9')
			return KEY_9;


		else if ((stricmp(_p, "SPACE")) == 0)
			return KEY_SPACE;
		/*else if ((stricmp(_p, "MINUS")) == 0)
			return KEY_MINUS;
			else if ((stricmp(_p, "EQUAL")) == 0)
			return KEY_EQUAL;
			else if ((stricmp(_p, "LEFT BRACKET")) == 0)
			return KEY_LEFT_BRACKET;
			else if ((stricmp(_p, "RIGHT BRACKET")) == 0)
			return KEY_RIGHT_BRACKET;
			else if ((stricmp(_p, "BACKSLASH")) == 0)
			return KEY_BACKSLASH;
			else if ((stricmp(_p, "SEMICOLON")) == 0)
			return KEY_SEMICOLON;
			else if ((stricmp(_p, "APOSTROPHE")) == 0)
			return KEY_APOSTROPHE;
			else if ((stricmp(_p, "GRAVE ACCENT")) == 0)
			return KEY_GRAVE_ACCENT;
			else if ((stricmp(_p, "COMMA")) == 0)
			return KEY_COMMA;
			else if ((stricmp(_p, "PERIOD")) == 0)
			return KEY_PERIOD;
			else if ((stricmp(_p, "SLASH")) == 0)
			return KEY_SLASH;
			else if ((stricmp(_p, "WORLD 1")) == 0)
			return KEY_WORLD_1;
			else if ((stricmp(_p, "WORLD 2")) == 0)
			return KEY_WORLD_2;


			else if ((stricmp(_p, "F1")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F2")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F3")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F4")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F5")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F6")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F7")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F8")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F9")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F10")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F11")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F12")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F13")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F14")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F15")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F16")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F17")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F18")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F19")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F20")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F21")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F22")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F23")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F24")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F25")) == 0)
			return KEY_ESC;
			else if ((stricmp(_p, "F26")) == 0)
			return KEY_ESC;*/

#if 0
		case GLFW_KEY_LEFT_SHIFT:   return "LEFT SHIFT";
		case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT SHIFT";
		case GLFW_KEY_LEFT_CONTROL: return "LEFT CONTROL";
		case GLFW_KEY_RIGHT_CONTROL: return "RIGHT CONTROL";
		case GLFW_KEY_LEFT_ALT:     return "LEFT ALT";
		case GLFW_KEY_RIGHT_ALT:    return "RIGHT ALT";
		case GLFW_KEY_TAB:          return "TAB";
		case GLFW_KEY_ENTER:        return "ENTER";
		case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
		case GLFW_KEY_INSERT:       return "INSERT";
		case GLFW_KEY_DELETE:       return "DELETE";
		case GLFW_KEY_PAGE_UP:      return "PAGE UP";
		case GLFW_KEY_PAGE_DOWN:    return "PAGE DOWN";
		case GLFW_KEY_HOME:         return "HOME";
		case GLFW_KEY_END:          return "END";
		case GLFW_KEY_KP_0:         return "KEYPAD 0";
		case GLFW_KEY_KP_1:         return "KEYPAD 1";
		case GLFW_KEY_KP_2:         return "KEYPAD 2";
		case GLFW_KEY_KP_3:         return "KEYPAD 3";
		case GLFW_KEY_KP_4:         return "KEYPAD 4";
		case GLFW_KEY_KP_5:         return "KEYPAD 5";
		case GLFW_KEY_KP_6:         return "KEYPAD 6";
		case GLFW_KEY_KP_7:         return "KEYPAD 7";
		case GLFW_KEY_KP_8:         return "KEYPAD 8";
		case GLFW_KEY_KP_9:         return "KEYPAD 9";
		case GLFW_KEY_KP_DIVIDE:    return "KEYPAD DIVIDE";
		case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD MULTPLY";
		case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD SUBTRACT";
		case GLFW_KEY_KP_ADD:       return "KEYPAD ADD";
		case GLFW_KEY_KP_DECIMAL:   return "KEYPAD DECIMAL";
		case GLFW_KEY_KP_EQUAL:     return "KEYPAD EQUAL";
		case GLFW_KEY_KP_ENTER:     return "KEYPAD ENTER";
		case GLFW_KEY_PRINT_SCREEN: return "PRINT SCREEN";
		case GLFW_KEY_NUM_LOCK:     return "NUM LOCK";
		case GLFW_KEY_CAPS_LOCK:    return "CAPS LOCK";
		case GLFW_KEY_SCROLL_LOCK:  return "SCROLL LOCK";
		case GLFW_KEY_PAUSE:        return "PAUSE";
		case GLFW_KEY_LEFT_SUPER:   return "LEFT SUPER";
		case GLFW_KEY_RIGHT_SUPER:  return "RIGHT SUPER";
		case GLFW_KEY_MENU:         return "MENU";
		case GLFW_KEY_UNKNOWN:      return "UNKNOWN";
#endif



		else if ((stricmp(_p, "ESC")) == 0)
			return KEY_ESC;
		else if ((stricmp(_p, "UP")) == 0)
			return KEY_UP;
		else if ((stricmp(_p, "DONW")) == 0)
			return KEY_DOWN;
		else if ((stricmp(_p, "LETF")) == 0)
			return KEY_LEFT;
		else if ((stricmp(_p, "RIGHT")) == 0)
			return KEY_RIGHT;

		return 0;
	}

	/**
	*/
	const char* get_action_name(int action)
	{
		switch (action)
		{
		case GLFW_PRESS:
			return "pressed";
		case GLFW_RELEASE:
			return "released";
		case GLFW_REPEAT:
			return "repeated";
		}

		return "caused unknown action";
	}

	/*
	*/
	const char* get_button_name(int button)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			return "left";
		case GLFW_MOUSE_BUTTON_RIGHT:
			return "right";
		case GLFW_MOUSE_BUTTON_MIDDLE:
			return "middle";
		}

		return NULL;
	}

	/*
	*/
	const char* get_mods_name(int mods)
	{
		static char name[512];

		name[0] = '\0';

		if (mods & GLFW_MOD_SHIFT)
			strcat(name, " shift");
		if (mods & GLFW_MOD_CONTROL)
			strcat(name, " control");
		if (mods & GLFW_MOD_ALT)
			strcat(name, " alt");
		if (mods & GLFW_MOD_SUPER)
			strcat(name, " super");

		return name;
	}

	/*
	*/
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		const char* name = get_key_name(key);
		auto windowSystem = (WindowSystemGLFW*)GetWindow();

		switch (action){
		case GLFW_PRESS:
		{
			windowSystem->keys[key] = true;
			break;
		}

		case GLFW_RELEASE:
		{
			windowSystem->keys[key] = false;
			break;
		}
		}
	}

	/**
	*/
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		const char* name = get_button_name(button);
		auto windowSystem = (WindowSystemGLFW*)GetWindow();

		switch (action){
		case GLFW_PRESS:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				windowSystem->mouseButtons[0] = true;
				if (!windowSystem->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMousePress(windowSystem->oldMouseX, windowSystem->oldMouseY, (MyGUI::MouseButton::Enum)0);
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				windowSystem->mouseButtons[1] = true;
				if (!windowSystem->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMousePress(windowSystem->oldMouseX, windowSystem->oldMouseY, (MyGUI::MouseButton::Enum)1);
			}
			break;
		}

		case GLFW_RELEASE:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				windowSystem->mouseButtons[0] = false;
				if (!windowSystem->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseRelease(windowSystem->oldMouseX, windowSystem->oldMouseY, (MyGUI::MouseButton::Enum)0);
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				windowSystem->mouseButtons[1] = false;
				if (!windowSystem->mouseGrabed)
					MyGUI::InputManager::getInstancePtr()->injectMouseRelease(windowSystem->oldMouseX, windowSystem->oldMouseY, (MyGUI::MouseButton::Enum)1);
			}
			break;
		}
		}
	}

	/**
	*/
	void cursor_position_callback(GLFWwindow* window, double mx, double my)
	{
		auto win = (WindowSystemGLFW*)GetWindow();

		if ((win->oldMouseX == win->mouseX) && (win->oldMouseY == win->mouseY))
			win->mousing = false;
		else
			win->mousing = true;

		win->oldMouseX = win->mouseX;
		win->oldMouseY = win->mouseY;

#pragma message("BUG,если сильно крутануть,то выйдет за границы ")

		win->mouseX = mx;
		win->mouseY = my;

		if (!win->mouseGrabed)
			MyGUI::InputManager::getInstancePtr()->injectMouseMove(win->oldMouseX, win->oldMouseY, 0);
	}

	/**
	*/
	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		CVARManager* cvars = GetCvars();
		cvars->r_width = width;
		cvars->r_height = height;

		WindowSystemGLFW* windowSystem = (WindowSystemGLFW*)GetWindow();

		GetRender()->reshape(width, height);

		windowSystem->width = width;
		windowSystem->height = height;
	}

	/**
	*/
	void window_close_callback(GLFWwindow* window) {
		exit(0);
		//TODO:Выводить сообщение,как в DEUS EX
	}
}