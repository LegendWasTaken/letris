#ifndef LETRIS_WINDOW_H
#define LETRIS_WINDOW_H

#include <cstdint>
#include <string>
#include <functional>
#include <optional>

#include <GLFW/glfw3.h>

#include <common/logical_devices.h>

#include <util/opengl.h>
#include <glm/glm.hpp>

namespace let {
    class window_init {
    public:
        window_init();
    };
}

namespace let {
    class window {
    public:
        window(std::uint16_t width, std::uint16_t height, const std::string &title);

        [[nodiscard]] bool should_close() const noexcept;

        [[nodiscard]] let::logical::mouse mouse() const noexcept;

        [[nodiscard]] let::logical::keyboard keyboard() const noexcept;

        [[nodiscard]] glm::ivec2 resolution() const noexcept;

        void set_texture_callback(std::function<std::optional<std::uint32_t>()> texture_callback);

        void display_frame();

    private:
        static void _glfw_cursor_position_callback(GLFWwindow *window, double x, double y);

        static void _glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static void _glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        std::uint16_t _width;
        std::uint16_t _height;

        std::uint32_t _fullscreen_vert;
        std::uint32_t _fullscreen_frag;
        std::uint32_t _fullscreen_program;

        std::function<std::optional<std::uint32_t>()> _texture_callback;

        GLFWwindow *_window;

        let::logical::mouse _mouse;

        let::logical::keyboard _keyboard;
    };
}

#endif //LETRIS_WINDOW_H
