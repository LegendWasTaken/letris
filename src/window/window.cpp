#include "window.h"

namespace
{
    void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam )
    {
        if (severity != 0x826b && severity != 3)
        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                 ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                 type, severity, message );
    }
}

let::window_init::window_init() {
}

let::window::window(std::uint16_t width, std::uint16_t height, const std::string &title, let::opengl::manager *manager) :
        _width(width), _height(height), _opengl_manager(manager) {
    ZoneScopedN("window::constructor");

    // Todo: Figure out how to move this out and make it static, problem is gladLoad must be after make context current...
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    _window = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    TracyGpuContext;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glfwSetWindowUserPointer(_window, this);

    glfwSetCursorPosCallback(_window, _glfw_cursor_position_callback);
    glfwSetMouseButtonCallback(_window, _glfw_mouse_button_callback);
    glfwSetKeyCallback(_window, _glfw_key_callback);

    _fullscreen_program = _opengl_manager->create_program({
        "shaders/fullscreen_texture/shader.vert",
        "shaders/fullscreen_texture/shader.frag"});
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

let::logical::mouse &let::window::mouse() noexcept {
    return _mouse;
}

let::logical::keyboard &let::window::keyboard() noexcept {
    return _keyboard;
}

glm::ivec2 let::window::resolution() const noexcept {
    ZoneScopedN("window::resolution");
    return glm::ivec2(_width, _height);
}

void let::window::display_frame(let::window::render_targets targets) {
    ZoneScopedN("window::display_frame");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _opengl_manager->bind(_fullscreen_program);
    _opengl_manager->uniform("has_world", targets.has_world);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, targets.gui);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, targets.rendered);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    TracyGpuCollect;
    glfwSwapBuffers(_window);

    glfwSetInputMode(_window, GLFW_CURSOR, _mouse.hidden() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    glfwPollEvents();
}

void let::window::_glfw_cursor_position_callback(GLFWwindow *window, double x, double y) {
    ZoneScopedN("window::cursor_position_callback");
    auto user_window = static_cast<let::window *>(glfwGetWindowUserPointer(window));
    user_window->_mouse.set_position({x, y});
}

void let::window::_glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    ZoneScopedN("window::mouse_button_callback");
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
    ZoneScopedN("window::key_callback");
    auto user_window = static_cast<let::window *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_RELEASE)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::released;
    else if (action == GLFW_REPEAT)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::repeat;
    else if (action == GLFW_PRESS)
        user_window->_keyboard.keys()[key].press_state = let::logical::keyboard::state::pressed;

    user_window->_keyboard.keys()[key].mods = mods;
}
