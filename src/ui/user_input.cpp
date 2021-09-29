#include "user_input.h"

void let::input_screen::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame,
                                   const ultralight::String &url) {
    auto context = caller->LockJSContext();
    ultralight::SetJSContext(context.get());

    auto global = ultralight::JSGlobalObject();

    for (auto &callback : manifest().callbacks)
        global[callback.name().c_str()] = callback.has_return_value() ? callback.ultralight_callback_with_ret()
                                                                      : callback.ultralight_callback_no_ret();
}

let::main_menu::main_menu() {
    _multiplayer_click_callback = js_callback("CPP_MultiplayerClick");
    _graphics_click_callback = js_callback("CPP_GraphicsClick");
    _sound_click_callback = js_callback("CPP_SoundClick");
}

let::input_screen::register_data let::main_menu::manifest() const noexcept {
    auto data = register_data();

    data.html_url = "file:///" + std::string(LETRIS_ASSET_PATH) + "/html/main_menu.html";

    data.callbacks.emplace_back(_multiplayer_click_callback);
    data.callbacks.emplace_back(_graphics_click_callback);
    data.callbacks.emplace_back(_sound_click_callback);

    return data;
}

void let::main_menu::on_multiplayer_click(const std::function<void()> &callback) {
    _multiplayer_click_callback.bind(callback);
}

void let::main_menu::on_graphics_click(const std::function<void()> &callback) {
    _graphics_click_callback.bind(callback);
}

void let::main_menu::on_sound_click(const std::function<void()> &callback) {
    _sound_click_callback.bind(callback);
}

let::user_input_renderer::user_input_renderer(
        ultralight::Ref<ultralight::Renderer> *renderer,
        glm::ivec2 size) :
        _renderer(renderer), _size(size), _current_screen(nullptr) {}

void let::user_input_renderer::set_resolution(std::uint16_t width, std::uint16_t height) {
}

void let::user_input_renderer::use(let::input_screen *screen) {
    _current_screen = screen;
    _current_view = _renderer->get().CreateView(_size.x, _size.y, false, nullptr);

    _current_view->get().LoadURL(_current_screen->manifest().html_url.c_str());
    _current_view->get().Focus();

    _current_view->get().set_load_listener(screen);
}

void let::user_input_renderer::update(const update_context &update_ctx) {

#ifndef NDEBUG
    if (update_ctx.keyboard.is_key_down(logical::keyboard::key_code::key_f9)) {
        // Reload the HTML
        _current_view->get().LoadURL(_current_screen->manifest().html_url.c_str());
    }
#endif

    _renderer->get().Update();

    if (_current_view.has_value()) {
        auto &view = _current_view.value().get();

        // Fire mouse events
        auto mouse_event = ultralight::MouseEvent();

        // Fire a mouse position update
        mouse_event.button = ultralight::MouseEvent::kButton_None;
        mouse_event.type = ultralight::MouseEvent::kType_MouseMoved;
        mouse_event.x = update_ctx.mouse.position().x;
        mouse_event.y = update_ctx.mouse.position().y;

        view.FireMouseEvent(mouse_event);

        // Go through each button, and fire the key
        const auto buttons = std::array<let::logical::mouse::button, 3>({
                                                                                logical::mouse::button::left,
                                                                                logical::mouse::button::right,
                                                                                logical::mouse::button::middle
                                                                        });

        const auto button_lut = std::array<ultralight::MouseEvent::Button, 3>({
                                                                                      ultralight::MouseEvent::kButton_Left,
                                                                                      ultralight::MouseEvent::kButton_Right,
                                                                                      ultralight::MouseEvent::kButton_Middle
                                                                              });

        const auto type_lut = std::array<ultralight::MouseEvent::Type, 2>({
                                                                                  ultralight::MouseEvent::kType_MouseDown,
                                                                                  ultralight::MouseEvent::kType_MouseUp,
                                                                          });

        if (_previous.update_ctx.has_value()) {
            for (auto button : buttons) {
                const auto button_state = update_ctx.mouse.state_of(button);
                if (button_state != _previous.update_ctx->mouse.state_of(button)) {
                    mouse_event.button = button_lut[static_cast<int>(button)];

                    mouse_event.type = type_lut[static_cast<int>(button_state)];

                    view.FireMouseEvent(mouse_event);
                }
            }
        }
        // The keys are hardcoded which is annoying, but not much to do about it.
        if (_previous.update_ctx.has_value()) {
            for (auto key = 32; key < 348; key++) {
                auto keyboard_event = ultralight::KeyEvent();

                const auto state = update_ctx.keyboard.state_of(static_cast<logical::keyboard::key_code>(key));
                const auto mods = update_ctx.keyboard.mods_of(static_cast<logical::keyboard::key_code>(key));

                if (state != _previous.update_ctx->keyboard.state_of(static_cast<logical::keyboard::key_code>(key))) {
                    if (state == let::logical::keyboard::state::pressed)
                        keyboard_event.type = ultralight::KeyEvent::kType_RawKeyDown;
                    else if (state == let::logical::keyboard::state::released)
                        keyboard_event.type = ultralight::KeyEvent::kType_KeyUp;
                    else {
                        continue; // Dont register other events with ultralight
                    }

                    // Todo: Javascript events for this stuff, not dealing with that yet

                    keyboard_event.virtual_key_code = lut::GLFWKeyCodeToUltralightKeyCode(
                            key); // Lets see if this needs no mapping (i expect it needs it)
                    keyboard_event.modifiers = lut::GLFWModsToUltralightMods(mods);
                    keyboard_event.native_key_code = 0;
                    GetKeyIdentifierFromVirtualKeyCode(keyboard_event.virtual_key_code, keyboard_event.key_identifier);

                    view.FireKeyEvent(keyboard_event);

                    // Check if we should fire text to be written
                    if (state != logical::keyboard::state::released) {
                        if (const auto text = lut::GLFWKeyCodeToChar(key); text != nullptr) {

                            auto modified = std::string(text);

                            if (mods & GLFW_MOD_SHIFT)
                                std::transform(modified.begin(), modified.end(), modified.begin(), [](char letter) {
                                    letter = ::toupper(letter); // Initial toupper, to get all of the chars
                                    letter = lut::apply_shift(
                                            letter); // Second pass to apply the shift to special chars

                                    return letter;
                                });

                            auto text_event = ultralight::KeyEvent();
                            text_event.type = ultralight::KeyEvent::kType_Char;
                            text_event.text = modified.c_str();
                            text_event.unmodified_text = text;
                            text_event.modifiers = lut::GLFWModsToUltralightMods(mods);
                            view.FireKeyEvent(text_event);
                        }
                    }

                }
            }
        }

        _previous.update_ctx = update_ctx;
    }
}

void let::user_input_renderer::render() {
    _renderer->get().Render();
}

void let::user_input_renderer::read_into(std::uint32_t texture) {
    if (_current_screen == nullptr)
        throw read_into_exception();

    const auto surface = _current_view->get().surface();
    const auto bitmap_surface = (ultralight::BitmapSurface *) surface;
    // Note: I don't know what to do about the C cast here, and I don't care enough

    const auto bitmap = bitmap_surface->bitmap();

    const auto pixels = bitmap->LockPixels();
    const auto width = bitmap->width();
    const auto height = bitmap->height();

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixels);

    bitmap->UnlockPixels();
}

let::js_callback::js_callback(std::string name) : _name(name) {

}

void let::js_callback::bind(const std::function<void()> &callback) {
    _empty_callback = callback;
}

void let::js_callback::bind(const std::function<void(ultralight::JSArgs)> &callback) {
    _argument_no_ret_callback = callback;
}

void let::js_callback::bind(const std::function<ultralight::JSValue(ultralight::JSArgs)> &callback) {
    _argument_ret_callback = callback;
}

bool let::js_callback::has_return_value() const noexcept {
    return _argument_ret_callback.has_value();
}

ultralight::JSCallback let::js_callback::ultralight_callback_no_ret() {

    if (_empty_callback.has_value()) {
        return [callback = _empty_callback.value()](const ultralight::JSObject &global, const ultralight::JSArgs &args) -> void {
            // Todo: Some type of warning here
            callback();
        };
    } else if (_argument_no_ret_callback.has_value()) {
        return [callback = _argument_no_ret_callback.value()](const ultralight::JSObject &global, const ultralight::JSArgs &args) -> void {
            callback(args);
        };
    } else
        throw no_callback_bound_exception();
}


ultralight::JSCallbackWithRetval let::js_callback::ultralight_callback_with_ret() {
    if (_argument_ret_callback.has_value())
    return [callback = _argument_ret_callback.value()](const ultralight::JSObject &global, const ultralight::JSArgs &args) -> ultralight::JSValue {
            return callback(args);
    };
    throw no_callback_bound_exception();
}

const std::string &let::js_callback::name() const noexcept {
    return _name;
}

let::graphics_menu::graphics_menu() {
    _main_menu_click_callback = js_callback("CPP_MainMenuClick");
}

let::input_screen::register_data let::graphics_menu::manifest() const noexcept {
    auto data = register_data();

    data.html_url = "file:///" + std::string(LETRIS_ASSET_PATH) + "/html/graphics_menu.html";

    data.callbacks.push_back(_main_menu_click_callback);

    return data;
}

void let::graphics_menu::on_main_menu_click(const std::function<void()> &callback) {
    _main_menu_click_callback.bind(callback);
}
