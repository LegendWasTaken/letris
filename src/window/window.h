#ifndef LETRIS_WINDOW_H
#define LETRIS_WINDOW_H

#include <cstdint>
#include <string>
#include <functional>
#include <optional>

#include <GLFW/glfw3.h>

#include <common/logical_devices.h>

#include <common/opengl/manager.h>
#include <glm/glm.hpp>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

namespace let {
    class window_init {
    public:
        window_init();
    };
}

namespace let {
    class window {
    public:
        window(std::uint16_t width, std::uint16_t height, const std::string &title, let::opengl::manager *manager);

        [[nodiscard]] bool should_close() const noexcept;

        [[nodiscard]] let::logical::mouse mouse() const noexcept;

        [[nodiscard]] let::logical::keyboard keyboard() const noexcept;

        [[nodiscard]] let::logical::mouse &mouse() noexcept;

        [[nodiscard]] let::logical::keyboard &keyboard() noexcept;

        [[nodiscard]] glm::ivec2 resolution() const noexcept;

        struct render_targets {
            std::uint32_t gui;
            std::uint32_t rendered;
            bool has_world;
        };

        void display_frame(render_targets);

    private:
        static void _glfw_cursor_position_callback(GLFWwindow *window, double x, double y);

        static void _glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static void _glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        std::uint16_t _width;
        std::uint16_t _height;

        std::uint64_t _fullscreen_program;

        let::opengl::manager *_opengl_manager;

        GLFWwindow *_window;

        let::logical::mouse _mouse;

        let::logical::keyboard _keyboard;
    };
}

#endif //LETRIS_WINDOW_H
