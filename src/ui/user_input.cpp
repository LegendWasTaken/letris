#include "user_input.h"

let::user_input_renderer::user_input_renderer(
        ultralight::Ref<ultralight::Renderer> *renderer,
        glm::ivec2 size) :
        _renderer(renderer), _size(size) {}

void let::user_input_renderer::set_resolution(std::uint16_t width, std::uint16_t height) {
}

void let::user_input_renderer::use(let::input_screen *screen) {
    _current_screen = screen;
    _current_view = _renderer->get().CreateView(_size.x, _size.y, false, nullptr);

    _current_view->get().LoadHTML(_current_screen->manifest().html_content.c_str());
    _current_view->get().Focus();
}

void let::user_input_renderer::update(const update_context &update_ctx) {
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

        // Fire keyboard events

//        const auto key_lut = std::array<int, 348 - 32>({
//           ultralight::KeyCodes::GK_SPACE,
//           ultralight::KeyCodes::GK_APPS,
//           ultralight::KeyCodes::GK_OEM_COMMA,
//           ultralight::KeyCodes::GK_OEM_MINUS,
//           ultralight::KeyCodes::GK_OEM_PERIOD,
//           ultralight::KeyCodes::GK_OEM_102,
//           ultralight::KeyCodes::GK_0,
//           ultralight::KeyCodes::GK_1,
//           ultralight::KeyCodes::GK_2,
//           ultralight::KeyCodes::GK_3,
//           ultralight::KeyCodes::GK_4,
//           ultralight::KeyCodes::GK_5,
//           ultralight::KeyCodes::GK_6,
//           ultralight::KeyCodes::GK_7,
//           ultralight::KeyCodes::GK_8,
//           ultralight::KeyCodes::GK_9,
//           ultralight::KeyCodes::GK_OEM_1,
//           ultralight::KeyCodes::GK_END, // No equal?
//           ultralight::KeyCodes::GK_A,
//           ultralight::KeyCodes::GK_B,
//           ultralight::KeyCodes::GK_C,
//           ultralight::KeyCodes::GK_D,
//           ultralight::KeyCodes::GK_E,
//           ultralight::KeyCodes::GK_F,
//           ultralight::KeyCodes::GK_G,
//           ultralight::KeyCodes::GK_I,
//           ultralight::KeyCodes::GK_J,
//           ultralight::KeyCodes::GK_K,
//           ultralight::KeyCodes::GK_L,
//           ultralight::KeyCodes::GK_M,
//           ultralight::KeyCodes::GK_N,
//           ultralight::KeyCodes::GK_O,
//           ultralight::KeyCodes::GK_P,
//           ultralight::KeyCodes::GK_Q,
//           ultralight::KeyCodes::GK_R,
//           ultralight::KeyCodes::GK_S,
//           ultralight::KeyCodes::GK_T,
//           ultralight::KeyCodes::GK_U,
//           ultralight::KeyCodes::GK_V,
//           ultralight::KeyCodes::GK_W,
//           ultralight::KeyCodes::GK_X,
//           ultralight::KeyCodes::GK_Y,
//           ultralight::KeyCodes::GK_Z,
//           ultralight::KeyCodes::GK_OEM_4,
//           ultralight::KeyCodes::GK_OEM_5,
//           ultralight::KeyCodes::GK_OEM_6,
//           ultralight::KeyCodes::GK_OEM_7,
//           ultralight::KeyCodes::GK_OEM_8, // Todo: ?????
//           ultralight::KeyCodes::GK_OEM_8,
//           ultralight::KeyCodes::GK_ESCAPE,
//           ultralight::KeyCodes::GK_ENTER
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//           ultralight::KeyCodes::GK_
//        });

        // The keys are hardcoded which is annoying, but not much to do about it.
        if (_previous.update_ctx.has_value()) {
            for (auto key = 32; key < 348; key++) {
                auto keyboard_event = ultralight::KeyEvent();

                const auto state = update_ctx.keyboard.state_of(static_cast<logical::keyboard::key_code>(key));

                if (state != _previous.update_ctx->keyboard.state_of(static_cast<logical::keyboard::key_code>(key))) {
                    if (state == let::logical::keyboard::state::pressed)
                        keyboard_event.type = ultralight::KeyEvent::kType_RawKeyDown;
                    else if (state == let::logical::keyboard::state::released)
                        keyboard_event.type = ultralight::KeyEvent::kType_KeyUp;
                    else {
                        continue; // Dont register other events with ultralight
                    }

                    keyboard_event.virtual_key_code = ultralight::KeyCodes::GK_E; // Lets see if this needs no mapping (i expect it needs it)
                    keyboard_event.modifiers = 0;
                    keyboard_event.native_key_code = 0;
                    GetKeyIdentifierFromVirtualKeyCode(keyboard_event.virtual_key_code, keyboard_event.key_identifier);

                    view.FireKeyEvent(keyboard_event);
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
    const auto bitmap_surface = (ultralight::BitmapSurface*) surface;
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
