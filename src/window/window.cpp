#include "window.h"

#include <fmt/core.h>

let::window_init::window_init() {
}

let::window::window(std::uint16_t width, std::uint16_t height, const std::string &title) :
        _width(width), _height(height) {

    // Todo: Figure out how to move this out and make it static, problem is gladLoad must be after make context current...
    glfwInit();

    _window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    _texture_callback = []() -> std::optional<std::uint32_t> { return std::nullopt; };

    glfwSetWindowUserPointer(_window, this);

    glfwSetCursorPosCallback(_window, _glfw_cursor_position_callback);
    glfwSetMouseButtonCallback(_window, _glfw_mouse_button_callback);
    glfwSetKeyCallback(_window, _glfw_key_callback);

    try {
        _fullscreen_vert = let::opengl::create_shader(
                std::string(LETRIS_ASSET_PATH) + "shaders/fullscreen_texture/shader.vert");
        _fullscreen_frag = let::opengl::create_shader(
                std::string(LETRIS_ASSET_PATH) + "shaders/fullscreen_texture/shader.frag");
        _fullscreen_program = let::opengl::create_program(_fullscreen_vert, _fullscreen_frag);
    } catch (opengl::shader_compile_exception &ex) {
        fmt::print("Exception creating shader: {}, with error: \n{}\n", ex.path(), ex.error());
    } catch (opengl::program_link_exception &ex) {
        fmt::print("Exception linking program: {}, with error: \n{}\n", ex.pid(), ex.error());
    }
}

bool let::window::should_close() const noexcept {
    return glfwWindowShouldClose(_window);
}

let::logical::mouse let::window::mouse() const noexcept {
    return _mouse;
}

let::logical::keyboard let::window::keyboard() const noexcept {
    return _keyboard;
}

void let::window::set_texture_callback(std::function<std::optional<std::uint32_t>()> texture_callback) {
    _texture_callback = std::move(texture_callback);
}

void let::window::display_frame() {
    glfwMakeContextCurrent(_window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto texture_handle = _texture_callback();

    glUseProgram(_fullscreen_program);

    glUniform1i(1, texture_handle.has_value());

    if (texture_handle.has_value()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_handle.value());
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void let::window::_glfw_cursor_position_callback(GLFWwindow *window, double x, double y) {
    auto user_window = static_cast<let::window *>(glfwGetWindowUserPointer(window));
    user_window->_mouse.set_position({x, y});
}

void let::window::_glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    auto user_window = static_cast<let::window *>(glfwGetWindowUserPointer(window));

    auto mouse_button = let::logical::mouse::button::left;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouse_button = let::logical::mouse::button::left;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouse_button = let::logical::mouse::button::right;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouse_button = let::logical::mouse::button::middle;
            break;
        default:;
    }

    auto mouse_action = let::logical::mouse::button_state::down;
    switch (action) {
        case GLFW_PRESS:
            mouse_action = let::logical::mouse::button_state::down;
            break;
        case GLFW_RELEASE:
            mouse_action = let::logical::mouse::button_state::up;
            break;
        default: ;
    }

    user_window->_mouse.buttons()[static_cast<int>(mouse_button)] = mouse_action;
}

void let::window::_glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto user_window = static_cast<let::window *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_RELEASE)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::released;
    else if (action == GLFW_REPEAT)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::repeat;
    else if (action == GLFW_PRESS)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::pressed;

    user_window->_keyboard.keys()[key].mods = mods;
}
