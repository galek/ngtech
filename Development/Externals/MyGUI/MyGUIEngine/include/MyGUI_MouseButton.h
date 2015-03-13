/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_MOUSE_BUTTON_H__
#define __MYGUI_MOUSE_BUTTON_H__

namespace MyGUI
{

	struct MYGUI_EXPORT MouseButton
	{
		enum Enum
		{
			MB_None = -1,

			MB_Left=0,
			MB_Right,
			MB_Middle,

			MB_Button0=0,
			MB_Button1,
			MB_Button2,
			MB_Button3,
			MB_Button4,
			MB_Button5,
			MB_Button6,
			MB_Button7,
			MB_MAX
		};

		MouseButton(Enum _value = MB_None) :
			mValue(_value)
		{
		}

		friend bool operator == (MouseButton const& a, MouseButton const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (MouseButton const& a, MouseButton const& b)
		{
			return a.mValue != b.mValue;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_MOUSE_BUTTON_H__
