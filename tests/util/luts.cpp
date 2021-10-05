#include <catch2/catch_all.hpp>

#include <util/luts.h>

TEST_CASE("GLFW Mods -> Ultralight Mods ", "[Util.Lut]") {
    GIVEN("A GLFW Alt modification") {
        auto mod = GLFW_MOD_ALT;
        REQUIRE(let::lut::GLFWModsToUltralightMods(mod) == ultralight::KeyEvent::kMod_AltKey);
    }

    GIVEN("A GLFW Ctrl modification") {
        auto mod = GLFW_MOD_CONTROL;
        REQUIRE(let::lut::GLFWModsToUltralightMods(mod) == ultralight::KeyEvent::kMod_CtrlKey);
    }

    GIVEN("A GLFW Super modification") {
        auto mod = GLFW_MOD_SUPER;
        REQUIRE(let::lut::GLFWModsToUltralightMods(mod) == ultralight::KeyEvent::kMod_MetaKey);
    }

    GIVEN("A GLFW Shift modification") {
        auto mod = GLFW_MOD_SHIFT;
        REQUIRE(let::lut::GLFWModsToUltralightMods(mod) == ultralight::KeyEvent::kMod_ShiftKey);
    }
}

TEST_CASE("Apply the shift key", "[Util.Lut]") {
    GIVEN("The number keys") {
        auto numbers = std::array<char, 10>({
            '0',
            '1',
            '2',
            '3',
            '4',
            '5',
            '6',
            '7',
            '8',
            '9',
        });

        WHEN("Shift is applied to them") {
            REQUIRE(let::lut::apply_shift(numbers[0]) == ')');
            REQUIRE(let::lut::apply_shift(numbers[1]) == '!');
            REQUIRE(let::lut::apply_shift(numbers[2]) == '@');
            REQUIRE(let::lut::apply_shift(numbers[3]) == '#');
            REQUIRE(let::lut::apply_shift(numbers[4]) == '$');
            REQUIRE(let::lut::apply_shift(numbers[5]) == '%');
            REQUIRE(let::lut::apply_shift(numbers[6]) == '^');
            REQUIRE(let::lut::apply_shift(numbers[7]) == '&');
            REQUIRE(let::lut::apply_shift(numbers[8]) == '*');
            REQUIRE(let::lut::apply_shift(numbers[9]) == '(');
        }
    }

    GIVEN("The other shiftable keys") {
        auto keys = std::array<char, 9>({
            '\'',
            ',',
            '-',
            '.',
            '/',
            ';',
            '[',
            '\\',
            ']',
        });

        REQUIRE(let::lut::apply_shift(keys[0]) == '"');
        REQUIRE(let::lut::apply_shift(keys[1]) == '<');
        REQUIRE(let::lut::apply_shift(keys[2]) == '_');
        REQUIRE(let::lut::apply_shift(keys[3]) == '>');
        REQUIRE(let::lut::apply_shift(keys[4]) == '?');
        REQUIRE(let::lut::apply_shift(keys[5]) == ':');
        REQUIRE(let::lut::apply_shift(keys[6]) == '{');
        REQUIRE(let::lut::apply_shift(keys[7]) == '|');
        REQUIRE(let::lut::apply_shift(keys[8]) == '}');
    }
}

TEST_CASE("GLFW Keycode to Char", "[Util.Lut]") {
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_SPACE)) == " ");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_APOSTROPHE)) == "'");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_COMMA)) == ",");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_MINUS)) == "-");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_PERIOD)) == ".");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_SLASH)) == "/");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_0)) == "0");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_1)) == "1");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_2)) == "2");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_3)) == "3");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_4)) == "4");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_5)) == "5");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_6)) == "6");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_7)) == "7");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_8)) == "8");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_9)) == "9");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_SEMICOLON)) == ";");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_EQUAL)) == "=");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_A)) == "a");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_B)) == "b");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_C)) == "c");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_D)) == "d");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_E)) == "e");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_F)) == "f");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_G)) == "g");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_H)) == "h");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_I)) == "i");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_J)) == "j");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_K)) == "k");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_L)) == "l");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_M)) == "m");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_N)) == "n");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_O)) == "o");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_P)) == "p");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_Q)) == "q");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_R)) == "r");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_S)) == "s");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_T)) == "t");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_U)) == "u");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_V)) == "v");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_W)) == "w");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_X)) == "x");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_Y)) == "y");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_Z)) == "z");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_LEFT_BRACKET)) == "[");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_BACKSLASH)) == "\\");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_RIGHT_BRACKET)) == "]");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_GRAVE_ACCENT)) == "`");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_ENTER)) == "\n");
    REQUIRE(std::string(let::lut::GLFWKeyCodeToChar(GLFW_KEY_TAB)) == "\t");
}

TEST_CASE("GLFW Keycode to Ultralight Keycode", "[Util.Lut]") {
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_SPACE) == ultralight::KeyCodes::GK_SPACE);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_APOSTROPHE) == ultralight::KeyCodes::GK_OEM_7);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_COMMA) == ultralight::KeyCodes::GK_OEM_COMMA);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_MINUS) == ultralight::KeyCodes::GK_OEM_MINUS);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_PERIOD) == ultralight::KeyCodes::GK_OEM_PERIOD);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_SLASH) == ultralight::KeyCodes::GK_OEM_2);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_0) == ultralight::KeyCodes::GK_0);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_1) == ultralight::KeyCodes::GK_1);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_2) == ultralight::KeyCodes::GK_2);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_3) == ultralight::KeyCodes::GK_3);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_4) == ultralight::KeyCodes::GK_4);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_5) == ultralight::KeyCodes::GK_5);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_6) == ultralight::KeyCodes::GK_6);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_7) == ultralight::KeyCodes::GK_7);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_8) == ultralight::KeyCodes::GK_8);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_9) == ultralight::KeyCodes::GK_9);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_SEMICOLON) == ultralight::KeyCodes::GK_OEM_1);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_EQUAL) == ultralight::KeyCodes::GK_OEM_PLUS);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_A) == ultralight::KeyCodes::GK_A);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_B) == ultralight::KeyCodes::GK_B);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_C) == ultralight::KeyCodes::GK_C);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_D) == ultralight::KeyCodes::GK_D);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_E) == ultralight::KeyCodes::GK_E);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F) == ultralight::KeyCodes::GK_F);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_G) == ultralight::KeyCodes::GK_G);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_H) == ultralight::KeyCodes::GK_H);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_I) == ultralight::KeyCodes::GK_I);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_J) == ultralight::KeyCodes::GK_J);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_K) == ultralight::KeyCodes::GK_K);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_L) == ultralight::KeyCodes::GK_L);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_M) == ultralight::KeyCodes::GK_M);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_N) == ultralight::KeyCodes::GK_N);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_O) == ultralight::KeyCodes::GK_O);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_P) == ultralight::KeyCodes::GK_P);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_Q) == ultralight::KeyCodes::GK_Q);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_R) == ultralight::KeyCodes::GK_R);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_S) == ultralight::KeyCodes::GK_S);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_T) == ultralight::KeyCodes::GK_T);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_U) == ultralight::KeyCodes::GK_U);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_V) == ultralight::KeyCodes::GK_V);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_W) == ultralight::KeyCodes::GK_W);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_X) == ultralight::KeyCodes::GK_X);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_Y) == ultralight::KeyCodes::GK_Y);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_Z) == ultralight::KeyCodes::GK_Z);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT_BRACKET) == ultralight::KeyCodes::GK_OEM_4);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_BACKSLASH) == ultralight::KeyCodes::GK_OEM_5);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT_BRACKET) == ultralight::KeyCodes::GK_OEM_6);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_GRAVE_ACCENT) == ultralight::KeyCodes::GK_OEM_3);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_WORLD_1) == ultralight::KeyCodes::GK_UNKNOWN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_WORLD_2) == ultralight::KeyCodes::GK_UNKNOWN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_ESCAPE) == ultralight::KeyCodes::GK_ESCAPE);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_ENTER) == ultralight::KeyCodes::GK_RETURN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_TAB) == ultralight::KeyCodes::GK_TAB);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_BACKSPACE) == ultralight::KeyCodes::GK_BACK);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_INSERT) == ultralight::KeyCodes::GK_INSERT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_DELETE) == ultralight::KeyCodes::GK_DELETE);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT) == ultralight::KeyCodes::GK_RIGHT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT) == ultralight::KeyCodes::GK_LEFT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_DOWN) == ultralight::KeyCodes::GK_DOWN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_UP) == ultralight::KeyCodes::GK_UP);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_PAGE_UP) == ultralight::KeyCodes::GK_PRIOR);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_PAGE_DOWN) == ultralight::KeyCodes::GK_NEXT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_HOME) == ultralight::KeyCodes::GK_HOME);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_END) == ultralight::KeyCodes::GK_END);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_CAPS_LOCK) == ultralight::KeyCodes::GK_CAPITAL);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_SCROLL_LOCK) == ultralight::KeyCodes::GK_SCROLL);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_NUM_LOCK) == ultralight::KeyCodes::GK_NUMLOCK);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_PRINT_SCREEN) == ultralight::KeyCodes::GK_SNAPSHOT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_PAUSE) == ultralight::KeyCodes::GK_PAUSE);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F1) == ultralight::KeyCodes::GK_F1);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F2) == ultralight::KeyCodes::GK_F2);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F3) == ultralight::KeyCodes::GK_F3);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F4) == ultralight::KeyCodes::GK_F4);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F5) == ultralight::KeyCodes::GK_F5);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F6) == ultralight::KeyCodes::GK_F6);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F7) == ultralight::KeyCodes::GK_F7);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F8) == ultralight::KeyCodes::GK_F8);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F9) == ultralight::KeyCodes::GK_F9);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F10) == ultralight::KeyCodes::GK_F10);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F11) == ultralight::KeyCodes::GK_F11);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F12) == ultralight::KeyCodes::GK_F12);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F13) == ultralight::KeyCodes::GK_F13);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F14) == ultralight::KeyCodes::GK_F14);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F15) == ultralight::KeyCodes::GK_F15);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F16) == ultralight::KeyCodes::GK_F16);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F17) == ultralight::KeyCodes::GK_F17);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F18) == ultralight::KeyCodes::GK_F18);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F19) == ultralight::KeyCodes::GK_F19);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F20) == ultralight::KeyCodes::GK_F20);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F21) == ultralight::KeyCodes::GK_F21);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F22) == ultralight::KeyCodes::GK_F22);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F23) == ultralight::KeyCodes::GK_F23);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F24) == ultralight::KeyCodes::GK_F24);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_F25) == ultralight::KeyCodes::GK_UNKNOWN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_0) == ultralight::KeyCodes::GK_NUMPAD0);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_1) == ultralight::KeyCodes::GK_NUMPAD1);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_2) == ultralight::KeyCodes::GK_NUMPAD2);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_3) == ultralight::KeyCodes::GK_NUMPAD3);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_4) == ultralight::KeyCodes::GK_NUMPAD4);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_5) == ultralight::KeyCodes::GK_NUMPAD5);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_6) == ultralight::KeyCodes::GK_NUMPAD6);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_7) == ultralight::KeyCodes::GK_NUMPAD7);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_8) == ultralight::KeyCodes::GK_NUMPAD8);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_9) == ultralight::KeyCodes::GK_NUMPAD9);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_DECIMAL) == ultralight::KeyCodes::GK_DECIMAL);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_DIVIDE) == ultralight::KeyCodes::GK_DIVIDE);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_MULTIPLY) == ultralight::KeyCodes::GK_MULTIPLY);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_SUBTRACT) == ultralight::KeyCodes::GK_SUBTRACT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_ADD) == ultralight::KeyCodes::GK_ADD);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_ENTER) == ultralight::KeyCodes::GK_RETURN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_KP_EQUAL) == ultralight::KeyCodes::GK_OEM_PLUS);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT_SHIFT) == ultralight::KeyCodes::GK_SHIFT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT_CONTROL) == ultralight::KeyCodes::GK_CONTROL);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT_ALT) == ultralight::KeyCodes::GK_MENU);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_LEFT_SUPER) == ultralight::KeyCodes::GK_LWIN);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT_SHIFT) == ultralight::KeyCodes::GK_SHIFT);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT_CONTROL) == ultralight::KeyCodes::GK_CONTROL);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT_ALT) == ultralight::KeyCodes::GK_MENU);
    REQUIRE(let::lut::GLFWKeyCodeToUltralightKeyCode(GLFW_KEY_RIGHT_SUPER) == ultralight::KeyCodes::GK_RWIN);
}
