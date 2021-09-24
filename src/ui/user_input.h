#ifndef LETRIS_USER_INPUT_H
#define LETRIS_USER_INPUT_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <AppCore/JSHelpers.h>
#include <AppCore/Window.h>
#include <Ultralight/JavaScript.h>

#include <ui/logical_devices.h>

#include <util/luts.h>
#include <util/files.h>

#include <string>
#include <optional>

#include <fmt/core.h>

namespace let {

    class ultralight_init {
    public:
        ultralight_init() {
            auto cfg = ultralight::Config();
            cfg.resource_path = "./resources";
            cfg.use_gpu_renderer = false;
            cfg.device_scale = 1.0;
            ultralight::Platform::instance().set_config(cfg);

            ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
            ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
            ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));
        }
    };

    class input_screen : public ultralight::LoadListener {
    public:

        struct js_callback {
            std::string name;
            std::function<ultralight::JSValue(const ultralight::JSObject &, const ultralight::JSArgs)> callback;
        };

        input_screen() = default;

        struct register_data {
            std::string html_content;
            std::vector<js_callback> callbacks
        };

        [[nodiscard]] virtual register_data manifest() const noexcept = 0;

        void OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame,
                        const ultralight::String &url) override;

        ultralight::JSValue GetMessage(const ultralight::JSObject &global_this, const ultralight::JSArgs &args) {
            return ultralight::JSValue(std::to_string(rand()).c_str());
        }
    };

    class test_screen : public input_screen {

        [[nodiscard]] input_screen::register_data manifest() const noexcept override {
            auto data = register_data();

            data.html_content = let::read_file(std::string(LETRIS_ASSET_PATH) + "/html/test_screen.html");

            return data;
        }
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
