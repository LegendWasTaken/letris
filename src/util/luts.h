#ifndef LETRIS_LUTS_H
#define LETRIS_LUTS_H

#include <GLFW/glfw3.h>
#include <Ultralight/Ultralight.h>

namespace let::lut {
    using namespace ultralight;
    using namespace ultralight::KeyCodes;

    [[nodiscard]] inline int GLFWModsToUltralightMods(int mods) {
        int result = 0;
        if (mods & GLFW_MOD_ALT)
            result |= KeyEvent::kMod_AltKey;
        if (mods & GLFW_MOD_CONTROL)
            result |= KeyEvent::kMod_CtrlKey;
        if (mods & GLFW_MOD_SUPER)
            result |= KeyEvent::kMod_MetaKey;
        if (mods & GLFW_MOD_SHIFT)
            result |= KeyEvent::kMod_ShiftKey;
        return result;
    }

    [[nodiscard]] inline char apply_shift(char letter) {
        switch (letter) {
            case '\'': return '"';
            case ',': return '<';
            case '-': return '_';
            case '.': return '>';
            case '/': return '?';
            case ';': return ':';
            case '[': return '{';
            case '\\': return '|';
            case ']': return '}';
            case '0': return ')';
            case '1': return '!';
            case '2': return '@';
            case '3': return '#';
            case '4': return '$';
            case '5': return '%';
            case '6': return '^';
            case '7': return '&';
            case '8': return '*';
            case '9': return '(';
            default:
                return letter;
        }
    }

    [[nodiscard]] inline const char * GLFWKeyCodeToChar(int key) {
        switch (key) {
            case GLFW_KEY_SPACE:
                return " ";
            case GLFW_KEY_APOSTROPHE:
                return "'";
            case GLFW_KEY_COMMA:
                return ",";
            case GLFW_KEY_MINUS:
                return "-";
            case GLFW_KEY_PERIOD:
                return ".";
            case GLFW_KEY_SLASH:
                return "/";
            case GLFW_KEY_0:
                return "0";
            case GLFW_KEY_1:
                return "1";
            case GLFW_KEY_2:
                return "2";
            case GLFW_KEY_3:
                return "3";
            case GLFW_KEY_4:
                return "4";
            case GLFW_KEY_5:
                return "5";
            case GLFW_KEY_6:
                return "6";
            case GLFW_KEY_7:
                return "7";
            case GLFW_KEY_8:
                return "8";
            case GLFW_KEY_9:
                return "9";
            case GLFW_KEY_SEMICOLON:
                return ";";
            case GLFW_KEY_EQUAL:
                return "=";
            case GLFW_KEY_A:
                return "a";
            case GLFW_KEY_B:
                return "b";
            case GLFW_KEY_C:
                return "c";
            case GLFW_KEY_D:
                return "d";
            case GLFW_KEY_E:
                return "e";
            case GLFW_KEY_F:
                return "f";
            case GLFW_KEY_G:
                return "g";
            case GLFW_KEY_H:
                return "h";
            case GLFW_KEY_I:
                return "i";
            case GLFW_KEY_J:
                return "j";
            case GLFW_KEY_K:
                return "k";
            case GLFW_KEY_L:
                return "l";
            case GLFW_KEY_M:
                return "m";
            case GLFW_KEY_N:
                return "n";
            case GLFW_KEY_O:
                return "o";
            case GLFW_KEY_P:
                return "p";
            case GLFW_KEY_Q:
                return "q";
            case GLFW_KEY_R:
                return "r";
            case GLFW_KEY_S:
                return "s";
            case GLFW_KEY_T:
                return "t";
            case GLFW_KEY_U:
                return "u";
            case GLFW_KEY_V:
                return "v";
            case GLFW_KEY_W:
                return "w";
            case GLFW_KEY_X:
                return "x";
            case GLFW_KEY_Y:
                return "y";
            case GLFW_KEY_Z:
                return "z";
            case GLFW_KEY_LEFT_BRACKET:
                return "[";
            case GLFW_KEY_BACKSLASH:
                return "\\";
            case GLFW_KEY_RIGHT_BRACKET:
                return "]";
            case GLFW_KEY_GRAVE_ACCENT:
                return "`";
            case GLFW_KEY_ENTER:
                return "\n";
            case GLFW_KEY_TAB:
                return "\t";
            default:
                return nullptr;
        }
    }

    [[nodiscard]] inline int GLFWKeyCodeToUltralightKeyCode(int key) {
        switch (key) {
            case GLFW_KEY_SPACE:
                return GK_SPACE;
            case GLFW_KEY_APOSTROPHE:
                return GK_OEM_7;
            case GLFW_KEY_COMMA:
                return GK_OEM_COMMA;
            case GLFW_KEY_MINUS:
                return GK_OEM_MINUS;
            case GLFW_KEY_PERIOD:
                return GK_OEM_PERIOD;
            case GLFW_KEY_SLASH:
                return GK_OEM_2;
            case GLFW_KEY_0:
                return GK_0;
            case GLFW_KEY_1:
                return GK_1;
            case GLFW_KEY_2:
                return GK_2;
            case GLFW_KEY_3:
                return GK_3;
            case GLFW_KEY_4:
                return GK_4;
            case GLFW_KEY_5:
                return GK_5;
            case GLFW_KEY_6:
                return GK_6;
            case GLFW_KEY_7:
                return GK_7;
            case GLFW_KEY_8:
                return GK_8;
            case GLFW_KEY_9:
                return GK_9;
            case GLFW_KEY_SEMICOLON:
                return GK_OEM_1;
            case GLFW_KEY_EQUAL:
                return GK_OEM_PLUS;
            case GLFW_KEY_A:
                return GK_A;
            case GLFW_KEY_B:
                return GK_B;
            case GLFW_KEY_C:
                return GK_C;
            case GLFW_KEY_D:
                return GK_D;
            case GLFW_KEY_E:
                return GK_E;
            case GLFW_KEY_F:
                return GK_F;
            case GLFW_KEY_G:
                return GK_G;
            case GLFW_KEY_H:
                return GK_H;
            case GLFW_KEY_I:
                return GK_I;
            case GLFW_KEY_J:
                return GK_J;
            case GLFW_KEY_K:
                return GK_K;
            case GLFW_KEY_L:
                return GK_L;
            case GLFW_KEY_M:
                return GK_M;
            case GLFW_KEY_N:
                return GK_N;
            case GLFW_KEY_O:
                return GK_O;
            case GLFW_KEY_P:
                return GK_P;
            case GLFW_KEY_Q:
                return GK_Q;
            case GLFW_KEY_R:
                return GK_R;
            case GLFW_KEY_S:
                return GK_S;
            case GLFW_KEY_T:
                return GK_T;
            case GLFW_KEY_U:
                return GK_U;
            case GLFW_KEY_V:
                return GK_V;
            case GLFW_KEY_W:
                return GK_W;
            case GLFW_KEY_X:
                return GK_X;
            case GLFW_KEY_Y:
                return GK_Y;
            case GLFW_KEY_Z:
                return GK_Z;
            case GLFW_KEY_LEFT_BRACKET:
                return GK_OEM_4;
            case GLFW_KEY_BACKSLASH:
                return GK_OEM_5;
            case GLFW_KEY_RIGHT_BRACKET:
                return GK_OEM_6;
            case GLFW_KEY_GRAVE_ACCENT:
                return GK_OEM_3;
            case GLFW_KEY_WORLD_1:
                return GK_UNKNOWN;
            case GLFW_KEY_WORLD_2:
                return GK_UNKNOWN;
            case GLFW_KEY_ESCAPE:
                return GK_ESCAPE;
            case GLFW_KEY_ENTER:
                return GK_RETURN;
            case GLFW_KEY_TAB:
                return GK_TAB;
            case GLFW_KEY_BACKSPACE:
                return GK_BACK;
            case GLFW_KEY_INSERT:
                return GK_INSERT;
            case GLFW_KEY_DELETE:
                return GK_DELETE;
            case GLFW_KEY_RIGHT:
                return GK_RIGHT;
            case GLFW_KEY_LEFT:
                return GK_LEFT;
            case GLFW_KEY_DOWN:
                return GK_DOWN;
            case GLFW_KEY_UP:
                return GK_UP;
            case GLFW_KEY_PAGE_UP:
                return GK_PRIOR;
            case GLFW_KEY_PAGE_DOWN:
                return GK_NEXT;
            case GLFW_KEY_HOME:
                return GK_HOME;
            case GLFW_KEY_END:
                return GK_END;
            case GLFW_KEY_CAPS_LOCK:
                return GK_CAPITAL;
            case GLFW_KEY_SCROLL_LOCK:
                return GK_SCROLL;
            case GLFW_KEY_NUM_LOCK:
                return GK_NUMLOCK;
            case GLFW_KEY_PRINT_SCREEN:
                return GK_SNAPSHOT;
            case GLFW_KEY_PAUSE:
                return GK_PAUSE;
            case GLFW_KEY_F1:
                return GK_F1;
            case GLFW_KEY_F2:
                return GK_F2;
            case GLFW_KEY_F3:
                return GK_F3;
            case GLFW_KEY_F4:
                return GK_F4;
            case GLFW_KEY_F5:
                return GK_F5;
            case GLFW_KEY_F6:
                return GK_F6;
            case GLFW_KEY_F7:
                return GK_F7;
            case GLFW_KEY_F8:
                return GK_F8;
            case GLFW_KEY_F9:
                return GK_F9;
            case GLFW_KEY_F10:
                return GK_F10;
            case GLFW_KEY_F11:
                return GK_F11;
            case GLFW_KEY_F12:
                return GK_F12;
            case GLFW_KEY_F13:
                return GK_F13;
            case GLFW_KEY_F14:
                return GK_F14;
            case GLFW_KEY_F15:
                return GK_F15;
            case GLFW_KEY_F16:
                return GK_F16;
            case GLFW_KEY_F17:
                return GK_F17;
            case GLFW_KEY_F18:
                return GK_F18;
            case GLFW_KEY_F19:
                return GK_F19;
            case GLFW_KEY_F20:
                return GK_F20;
            case GLFW_KEY_F21:
                return GK_F21;
            case GLFW_KEY_F22:
                return GK_F22;
            case GLFW_KEY_F23:
                return GK_F23;
            case GLFW_KEY_F24:
                return GK_F24;
            case GLFW_KEY_F25:
                return GK_UNKNOWN;
            case GLFW_KEY_KP_0:
                return GK_NUMPAD0;
            case GLFW_KEY_KP_1:
                return GK_NUMPAD1;
            case GLFW_KEY_KP_2:
                return GK_NUMPAD2;
            case GLFW_KEY_KP_3:
                return GK_NUMPAD3;
            case GLFW_KEY_KP_4:
                return GK_NUMPAD4;
            case GLFW_KEY_KP_5:
                return GK_NUMPAD5;
            case GLFW_KEY_KP_6:
                return GK_NUMPAD6;
            case GLFW_KEY_KP_7:
                return GK_NUMPAD7;
            case GLFW_KEY_KP_8:
                return GK_NUMPAD8;
            case GLFW_KEY_KP_9:
                return GK_NUMPAD9;
            case GLFW_KEY_KP_DECIMAL:
                return GK_DECIMAL;
            case GLFW_KEY_KP_DIVIDE:
                return GK_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY:
                return GK_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT:
                return GK_SUBTRACT;
            case GLFW_KEY_KP_ADD:
                return GK_ADD;
            case GLFW_KEY_KP_ENTER:
                return GK_RETURN;
            case GLFW_KEY_KP_EQUAL:
                return GK_OEM_PLUS;
            case GLFW_KEY_LEFT_SHIFT:
                return GK_SHIFT;
            case GLFW_KEY_LEFT_CONTROL:
                return GK_CONTROL;
            case GLFW_KEY_LEFT_ALT:
                return GK_MENU;
            case GLFW_KEY_LEFT_SUPER:
                return GK_LWIN;
            case GLFW_KEY_RIGHT_SHIFT:
                return GK_SHIFT;
            case GLFW_KEY_RIGHT_CONTROL:
                return GK_CONTROL;
            case GLFW_KEY_RIGHT_ALT:
                return GK_MENU;
            case GLFW_KEY_RIGHT_SUPER:
                return GK_RWIN;
            case GLFW_KEY_MENU:
                return GK_UNKNOWN;
            default:
                return GK_UNKNOWN;
        }
    }
}

#endif //LETRIS_LUTS_H
