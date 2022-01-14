#ifndef LETRIS_USER_INPUT_H
#define LETRIS_USER_INPUT_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <AppCore/JSHelpers.h>
#include <AppCore/Window.h>
#include <Ultralight/JavaScript.h>

#include <common/logical_devices.h>

#include <ui/ultralight_filesystem.h>

#include <util/luts.h>
#include <util/files.h>

#include <string>
#include <optional>
#include <utility>

#include <tracy/Tracy.hpp>

namespace let {

    class ultralight_init {
    private:
        let::ultralight_filesystem _filesystem {};
    public:
        ultralight_init() {
            auto cfg = ultralight::Config();
            cfg.resource_path = "./resources";
            cfg.use_gpu_renderer = false;
            cfg.device_scale = 1.0;
            ultralight::Platform::instance().set_config(cfg);

            ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
            ultralight::Platform::instance().set_file_system(&_filesystem);
            ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));
        }
    };

    class js_callback {
    private:
        std::optional<std::function<void()>> _empty_callback;
        std::optional<std::function<void(ultralight::JSArgs)>> _argument_no_ret_callback;
        std::optional<std::function<ultralight::JSValue(ultralight::JSArgs)>> _argument_ret_callback;

        std::string _name;

    public:
        struct no_callback_bound_exception : std::exception {

        };

        js_callback() = default;

        explicit js_callback(std::string name);

        void bind(const std::function<void()> &callback);

        void bind(const std::function<void(ultralight::JSArgs)> &callback);

        void bind(const std::function<ultralight::JSValue(ultralight::JSArgs)> &callback);

        [[nodiscard]] bool has_return_value() const noexcept;

        [[nodiscard]] ultralight::JSCallback ultralight_callback_no_ret();

        [[nodiscard]] ultralight::JSCallbackWithRetval ultralight_callback_with_ret();

        [[nodiscard]] const std::string &name() const noexcept;
    };

    class input_screen : public ultralight::LoadListener {
    public:
        struct register_data {
            std::string html_url;
            std::vector<js_callback> callbacks;
        };

        input_screen() = default;

        [[nodiscard]] virtual register_data manifest() const noexcept = 0;

        void OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame,
                        const ultralight::String &url) override;
    };

    class main_menu : public input_screen {
    private:
        js_callback _multiplayer_click_callback;
        js_callback _graphics_click_callback;
        js_callback _sound_click_callback;

    public:
        main_menu();

        [[nodiscard]] input_screen::register_data manifest() const noexcept override;

        void on_multiplayer_click(const std::function<void()> &callback);

        void on_graphics_click(const std::function<void()> &callback);

        void on_sound_click(const std::function<void()> &callback);
    };

    struct multiplayer_menu : public input_screen {
    private:
        js_callback _main_menu_click_callback;
        js_callback _server_data_callback;
        js_callback _server_click_callback;

    public:
        multiplayer_menu();

        [[nodiscard]] input_screen::register_data manifest() const noexcept override;

        void on_main_menu_click(const std::function<void()> &callback);

        void on_server_data_request(const std::function<ultralight::JSValue(ultralight::JSArgs)> &callback);

        void on_server_click(const std::function<void(ultralight::JSArgs)> &server_click);
    };

    class graphics_menu : public input_screen {
    private:
        js_callback _main_menu_click_callback;

    public:
        graphics_menu();

        [[nodiscard]] input_screen::register_data manifest() const noexcept override;

        void on_main_menu_click(const std::function<void()> &callback);
    };

    class debug_menu : public input_screen {
    private:

    public:
        debug_menu();

        [[nodiscard]] input_screen::register_data manifest() const noexcept override;
    };

    class user_input_renderer {
    public:
        user_input_renderer(ultralight::Ref<ultralight::Renderer> *renderer, glm::ivec2 size);

        void set_resolution(std::uint16_t width, std::uint16_t height);

        void use(input_screen *screen);

        struct update_context {
            let::logical::mouse mouse;
            let::logical::keyboard keyboard;
        };

        void update(const update_context &update_ctx);

        void render();

        class read_into_exception : public std::exception {
        };

        void read_into(std::uint32_t texture);

    private:
        input_screen *_current_screen;

        std::optional<ultralight::Ref<ultralight::View>> _current_view;

        struct {
            std::optional<update_context> update_ctx;
        } _previous;

        glm::ivec2 _size;

        ultralight::Ref<ultralight::Renderer> *_renderer;
    };
}

#endif //LETRIS_USER_INPUT_H
