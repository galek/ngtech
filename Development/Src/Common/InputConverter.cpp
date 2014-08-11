#include "CommonPrivate.h"

#include "IWindow.h"

namespace NGTech {
	/*
	Keyboard keys enum
	*/
	enum Key {
		KEY_ESC = VK_ESCAPE,
		KEY_SPACE = VK_SPACE,

		KEY_UP = VK_UP,
		KEY_DOWN = VK_DOWN,
		KEY_LEFT = VK_LEFT,
		KEY_RIGHT = VK_RIGHT,

		KEY_0 = VK_NUMPAD0,
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
	/*
	*/
	int Input_GetKeyValueByChar(const char*  _p)
	{
		if (stricmp(_p, "a") == 0)
			return KEY_A;
		else if (stricmp(_p, "b") == 0)
			return KEY_B;
		else if (stricmp(_p, "c") == 0)
			return KEY_C;
		else if (stricmp(_p, "d") == 0)
			return KEY_D;
		else if (stricmp(_p, "e") == 0)
			return KEY_E;
		else if (stricmp(_p, "f") == 0)
			return KEY_F;
		else if (stricmp(_p, "g") == 0)
			return KEY_G;
		else if (stricmp(_p, "h") == 0)
			return KEY_H;
		else if (stricmp(_p, "i") == 0)
			return KEY_I;
		else if (stricmp(_p, "j") == 0)
			return KEY_J;
		else if (stricmp(_p, "k") == 0)
			return KEY_K;
		else if (stricmp(_p, "l") == 0)
			return KEY_L;
		else if (stricmp(_p, "m") == 0)
			return KEY_M;
		else if (stricmp(_p, "n") == 0)
			return KEY_N;
		else if (stricmp(_p, "o") == 0)
			return KEY_O;
		else if (stricmp(_p, "p") == 0)
			return KEY_P;
		else if (stricmp(_p, "q") == 0)
			return KEY_Q;
		else if (stricmp(_p, "r") == 0)
			return KEY_R;
		else if (stricmp(_p, "s") == 0)
			return KEY_S;
		else if (stricmp(_p, "t") == 0)
			return KEY_T;
		else if (stricmp(_p, "v") == 0)
			return KEY_U;
		else if (stricmp(_p, "v") == 0)
			return KEY_V;
		else if (stricmp(_p, "w") == 0)
			return KEY_W;
		else if (stricmp(_p, "x") == 0)
			return KEY_X;
		else if (stricmp(_p, "y") == 0)
			return KEY_Y;
		else if (stricmp(_p, "z") == 0)
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
		
		else if ((stricmp(_p, "esc")) == 0)
			return KEY_ESC;
		else if ((stricmp(_p, "up")) == 0)
			return KEY_UP;
		else if ((stricmp(_p, "down")) == 0)
			return KEY_DOWN;
		else if ((stricmp(_p, "lefb")) == 0)
			return KEY_LEFT;
		else if ((stricmp(_p, "rigb")) == 0)
			return KEY_RIGHT;
	
		return 0;
	}
	/*
	*/
	char* Input_GetKeyValueByInt(int _p)
	{
		if (_p == KEY_A)
			return "a";
		else if (_p == KEY_B)
			return "b";
		else if (_p == KEY_C)
			return "c";
		else if (_p == KEY_D)
			return "d";
		else if (_p == KEY_E)
			return "e";
		else if (_p == KEY_F)
			return "f";
		else if (_p == KEY_G)
			return "g";
		else if (_p == KEY_H)
			return "h";
		else if (_p == KEY_I)
			return "i";
		else if (_p == KEY_J)
			return "j";
		else if (_p == KEY_K)
			return "k";
		else if (_p == KEY_L)
			return "l";
		else if (_p == KEY_M)
			return "m";
		else if (_p == KEY_N)
			return "n";
		else if (_p == KEY_O)
			return "o";
		else if (_p == KEY_P)
			return "p";
		else if (_p == KEY_Q)
			return "q";
		else if (_p == KEY_R)
			return "r";
		else if (_p == KEY_S)
			return "s";
		else if (_p == KEY_T)
			return "t";
		else if (_p == KEY_U)
			return "u";
		else if (_p == KEY_V)
			return "v";
		else if (_p == KEY_W)
			return "w";
		else if (_p == KEY_X)
			return "x";
		else if (_p == KEY_Y)
			return "y";
		else if (_p == KEY_Z)
			return "z";

		else if (_p == KEY_0)
			return "0";
		else if (_p == KEY_1)
			return "1";
		else if (_p == KEY_2)
			return "2";
		else if (_p == KEY_3)
			return "3";
		else if (_p == KEY_4)
			return "4";
		else if (_p == KEY_5)
			return "5";
		else if (_p == KEY_6)
			return "6";
		else if (_p == KEY_7)
			return "7";
		else if (_p == KEY_8)
			return "8";
		else if (_p == KEY_9)
			return "9";

		else if (_p == KEY_ESC)
			return "esc";
		else if (_p == KEY_SPACE)
			return "spc";
		else if (_p == KEY_UP)
			return "up";
		else if (_p == KEY_DOWN)
			return "down";
		else if (_p == KEY_LEFT)
			return "lefb";
		else if (_p == KEY_RIGHT)
			return "rigb";

		return "NULL";
	}
}