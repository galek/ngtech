/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_KEY_CODE_H__
#define __MYGUI_KEY_CODE_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI {

    struct MYGUI_EXPORT KeyCode {
        enum Enum {
            MK_None = 0x00,
            MK_Escape = 0x01,
            MK_One = 0x02,
            MK_Two = 0x03,
            MK_Three = 0x04,
            MK_Four = 0x05,
            MK_Five = 0x06,
            MK_Six = 0x07,
            MK_Seven = 0x08,
            MK_Eight = 0x09,
            MK_Nine = 0x0A,
            MK_Zero = 0x0B,
            MK_Minus = 0x0C, /* - on main keyboard */
                    MK_Equals = 0x0D,
            MK_Backspace = 0x0E, /* Backspace */
                    MK_Tab = 0x0F,
            MK_Q = 0x10,
            MK_W = 0x11,
            MK_E = 0x12,
            MK_R = 0x13,
            MK_T = 0x14,
            MK_Y = 0x15,
            MK_U = 0x16,
            MK_I = 0x17,
            MK_O = 0x18,
            MK_P = 0x19,
            MK_LeftBracket = 0x1A,
            MK_RightBracket = 0x1B,
            MK_Return = 0x1C, /* Enter on main keyboard */
                    MK_LeftControl = 0x1D,
            MK_A = 0x1E,
            MK_S = 0x1F,
            MK_D = 0x20,
            MK_F = 0x21,
            MK_G = 0x22,
            MK_H = 0x23,
            MK_J = 0x24,
            MK_K = 0x25,
            MK_L = 0x26,
            MK_Semicolon = 0x27,
            MK_Apostrophe = 0x28,
            MK_Grave = 0x29, /* Accent grave ( ~ ) */
                    MK_LeftShift = 0x2A,
            MK_Backslash = 0x2B,
            MK_Z = 0x2C,
            MK_X = 0x2D,
            MK_C = 0x2E,
            MK_V = 0x2F,
            MK_B = 0x30,
            MK_N = 0x31,
            MK_M = 0x32,
            MK_Comma = 0x33,
            MK_Period = 0x34, /* . on main keyboard */
                    MK_Slash = 0x35, /* '/' on main keyboard */
                    MK_RightShift = 0x36,
            MK_Multiply = 0x37, /* * on numeric keypad */
                    MK_LeftAlt = 0x38, /* Left Alt */
            MK_Space = 0x39,
            MK_Capital = 0x3A,
            MK_F1 = 0x3B,
            MK_F2 = 0x3C,
            MK_F3 = 0x3D,
            MK_F4 = 0x3E,
            MK_F5 = 0x3F,
            MK_F6 = 0x40,
            MK_F7 = 0x41,
            MK_F8 = 0x42,
            MK_F9 = 0x43,
            MK_F10 = 0x44,
            MK_NumLock = 0x45,
            MK_ScrollLock = 0x46, /* Scroll Lock */
            MK_Numpad7 = 0x47,
            MK_Numpad8 = 0x48,
            MK_Numpad9 = 0x49,
            MK_Subtract = 0x4A, /* - on numeric keypad */
                    MK_Numpad4 = 0x4B,
            MK_Numpad5 = 0x4C,
            MK_Numpad6 = 0x4D,
            MK_Add = 0x4E, /* + on numeric keypad */
                    MK_Numpad1 = 0x4F,
            MK_Numpad2 = 0x50,
            MK_Numpad3 = 0x51,
            MK_Numpad0 = 0x52,
            MK_Decimal = 0x53, /* . on numeric keypad */
                    MK_OEM_102 = 0x56, /* < > | on UK/Germany keyboards */
                    MK_F11 = 0x57,
            MK_F12 = 0x58,
            MK_F13 = 0x64, /*                     (NEC PC98) */
                    MK_F14 = 0x65, /*                     (NEC PC98) */
                    MK_F15 = 0x66, /*                     (NEC PC98) */
                    MK_Kana = 0x70, /* (Japanese keyboard)            */
                    MK_ABNT_C1 = 0x73, /* / ? on Portugese (Brazilian) keyboards */
                    MK_Convert = 0x79, /* (Japanese keyboard)            */
                    MK_NoConvert = 0x7B, /* (Japanese keyboard)            */
                    MK_Yen = 0x7D, /* (Japanese keyboard)            */
                    MK_ABNT_C2 = 0x7E, /* Numpad . on Portugese (Brazilian) keyboards */
                    MK_NumpadEquals = 0x8D, /* = on numeric keypad (NEC PC98) */
                    MK_PrevTrack = 0x90, /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
                    MK_At = 0x91, /*                     (NEC PC98) */
                    MK_Colon = 0x92, /*                     (NEC PC98) */
                    MK_Underline = 0x93, /*                     (NEC PC98) */
                    MK_Kanji = 0x94, /* (Japanese keyboard)            */
                    MK_Stop = 0x95, /*                     (NEC PC98) */
                    MK_AX = 0x96, /*                     (Japan AX) */
                    MK_Unlabeled = 0x97, /*                        (J3100) */
                    MK_NextTrack = 0x99, /* Next Track */
                    MK_NumpadEnter = 0x9C, /* Enter on numeric keypad */
                    MK_RightControl = 0x9D,
            MK_Mute = 0xA0,
            MK_Calculator = 0xA1,
            MK_PlayPause = 0xA2, /* Play / Pause */
                    MK_MediaStop = 0xA4, /* Media Stop */
                    MK_VolumeDown = 0xAE, /* Volume - */
                    MK_VolumeUp = 0xB0, /* Volume + */
                    MK_WebHome = 0xB2, /* Web home */
                    MK_NumpadComma = 0xB3, /* , on numeric keypad (NEC PC98) */
                    MK_Divide = 0xB5, /* / on numeric keypad */
                    MK_SysRq = 0xB7,
            MK_RightAlt = 0xB8, /* Right Alt */
                    MK_Pause = 0xC5,
            MK_Home = 0xC7, /* Home on arrow keypad */
                    MK_ArrowUp = 0xC8, /* UpArrow on arrow keypad */
                    MK_PageUp = 0xC9, /* PgUp on arrow keypad */
                    MK_ArrowLeft = 0xCB, /* LeftArrow on arrow keypad */
                    MK_ArrowRight = 0xCD, /* RightArrow on arrow keypad */
                    MK_End = 0xCF, /* End on arrow keypad */
                    MK_ArrowDown = 0xD0, /* DownArrow on arrow keypad */
                    MK_PageDown = 0xD1, /* PgDn on arrow keypad */
                    MK_Insert = 0xD2, /* Insert on arrow keypad */
                    MK_Delete = 0xD3, /* Delete on arrow keypad */
                    MK_LeftWindows = 0xDB, /* Left Windows key */
                    MK_RightWindow = 0xDC, /* Right Windows key */
                    MK_RightWindows = 0xDC, /* Right Windows key - Correct spelling :) */
                    MK_AppMenu = 0xDD, /* AppMenu key */
                    MK_Power = 0xDE, /* System Power */
                    MK_Sleep = 0xDF, /* System Sleep */
                    MK_Wake = 0xE3, /* System Wake */
                    MK_WebSearch = 0xE5,
            MK_WebFavorites = 0xE6,
            MK_WebRefresh = 0xE7,
            MK_WebStop = 0xE8,
            MK_WebForward = 0xE9,
            MK_WebBack = 0xEA,
            MK_MyComputer = 0xEB,
            MK_Mail = 0xEC,
            MK_MediaSelect = 0xED
        };

        KeyCode(Enum _value = MK_None) :
                mValue(_value) {
        }

        friend bool operator<(KeyCode const &a, KeyCode const &b) {
            return (a.mValue < b.mValue);
        }

        friend bool operator>=(KeyCode const &a, KeyCode const &b) {
            return !(a < b);
        }

        friend bool operator>(KeyCode const &a, KeyCode const &b) {
            return (b < a);
        }

        friend bool operator<=(KeyCode const &a, KeyCode const &b) {
            return !(a > b);
        }

        friend bool operator==(KeyCode const &a, KeyCode const &b) {
            return !(a < b) && !(a > b);
        }

        friend bool operator!=(KeyCode const &a, KeyCode const &b) {
            return !(a == b);
        }

        int getValue() const {
            return mValue;
        }

    private:
        Enum mValue;
    };

} // namespace MyGUI

#endif // __MYGUI_KEY_CODE_H__
