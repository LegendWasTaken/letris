#ifndef LETRIS_USER_INPUT_H
#define LETRIS_USER_INPUT_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>

#include <ui/logical_devices.h>

#include <string>
#include <optional>

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

    class input_screen {
    public:
        input_screen() = default;

        struct register_data {
            std::string html_content;
        };

        [[nodiscard]] virtual register_data manifest() const noexcept = 0;

    };

    class test_screen : public input_screen {
        [[nodiscard]] input_screen::register_data manifest() const noexcept override {
            return {.html_content = "<!DOCTYPE html>\n"
                                    "<html lang=\"en\">\n"
                                    "<meta charset=\"UTF-8\">\n"
                                    "<link href='https://fonts.googleapis.com/css?family=Raleway' rel='stylesheet'>\n"
                                    "<style>\n"
                                    "    body {\n"
                                    "        background-color: whitesmoke;\n"
                                    "        font-family: 'Raleway', serif;\n"
                                    "    }\n"
                                    "\n"
                                    "    .background {\n"
                                    "        display: flex;\n"
                                    "        flex-direction: row;\n"
                                    "    }\n"
                                    "\n"
                                    "    .selector {\n"
                                    "        display: flex;\n"
                                    "        flex-direction: row;\n"
                                    "        margin-left: 5%;\n"
                                    "        font-size: 24px;\n"
                                    "    }\n"
                                    "\n"
                                    "    .selector-list {\n"
                                    "        display: flex;\n"
                                    "        flex-direction: column;\n"
                                    "    }\n"
                                    "\n"
                                    "    .line-vertical {\n"
                                    "        border-left: 2px solid black;\n"
                                    "        height: 90vh;\n"
                                    "        margin-left: 2vh;\n"
                                    "    }\n"
                                    "\n"
                                    "    .line-horizontal {\n"
                                    "        border-top: 2px solid black;\n"
                                    "        width: 50vw;\n"
                                    "    }\n"
                                    "\n"
                                    "    .module {\n"
                                    "        margin-left: 5vw;\n"
                                    "    }\n"
                                    "\n"
                                    "    .unselectable {\n"
                                    "        user-select: none;\n"
                                    "    }\n"
                                    "\n"
                                    "    .selected {\n"
                                    "        font-weight: bold;\n"
                                    "        text-decoration: underline;\n"
                                    "    }\n"
                                    "\n"
                                    "    #module-title {\n"
                                    "        font-size: 36px;\n"
                                    "        font-weight: bold;\n"
                                    "    }\n"
                                    "\n"
                                    "    #module-description {\n"
                                    "        font-size: 14px;\n"
                                    "        font-style: italic;\n"
                                    "    }\n"
                                    "\n"
                                    "    .module-setting-container {\n"
                                    "        width: 100%;\n"
                                    "        content: \"\";\n"
                                    "        display: table;\n"
                                    "    }\n"
                                    "\n"
                                    "    .module-setting {\n"
                                    "        white-space: nowrap;\n"
                                    "        overflow: hidden;\n"
                                    "        font-size: 24px;\n"
                                    "        float: left;\n"
                                    "        width: 50%;\n"
                                    "    }\n"
                                    "\n"
                                    "    .slider-container {\n"
                                    "        margin: auto;\n"
                                    "        width: 100%;\n"
                                    "    }\n"
                                    "\n"
                                    "    .slider {\n"
                                    "        appearance: none;\n"
                                    "        width: 100%;\n"
                                    "        height: 25px;\n"
                                    "        background: #d3d3d3;\n"
                                    "        outline: none;\n"
                                    "        opacity: 0.7;\n"
                                    "        -webkit-transition: .2s;\n"
                                    "        transition: opacity .2s;\n"
                                    "        border-radius: 10px;\n"
                                    "    }\n"
                                    "\n"
                                    "    .slider::-moz-range-thumb {\n"
                                    "        width: 25px;\n"
                                    "        height: 25px;\n"
                                    "        border-radius: 10px;\n"
                                    "        background: black;\n"
                                    "        cursor: pointer;\n"
                                    "    }\n"
                                    "\n"
                                    "    .switch {\n"
                                    "        position: relative;\n"
                                    "        display: inline-block;\n"
                                    "        height: 25px;\n"
                                    "        width: 52px;\n"
                                    "    }\n"
                                    "\n"
                                    "    .switch input {\n"
                                    "        opacity: 0;\n"
                                    "        width: 0;\n"
                                    "        height: 0;\n"
                                    "    }\n"
                                    "\n"
                                    "    /* The slider */\n"
                                    "    .toggle-slider {\n"
                                    "        position: absolute;\n"
                                    "        cursor: pointer;\n"
                                    "        top: 0;\n"
                                    "        left: 0;\n"
                                    "        right: 0;\n"
                                    "        bottom: 0;\n"
                                    "        background-color: #ccc;\n"
                                    "        border-radius: 20px;\n"
                                    "        -webkit-transition: .4s;\n"
                                    "        transition: .4s;\n"
                                    "    }\n"
                                    "\n"
                                    "    .toggle-slider:before {\n"
                                    "        position: absolute;\n"
                                    "        content: \"\";\n"
                                    "        height: 18px;\n"
                                    "        width: 18px;\n"
                                    "        left: 4px;\n"
                                    "        bottom: 4px;\n"
                                    "        border-radius: 34px;\n"
                                    "        background-color: white;\n"
                                    "        -webkit-transition: .4s;\n"
                                    "        transition: .4s;\n"
                                    "    }\n"
                                    "\n"
                                    "    input:checked + .toggle-slider {\n"
                                    "        background-color: lawngreen;\n"
                                    "    }\n"
                                    "\n"
                                    "    input:focus + .toggle-slider {\n"
                                    "        box-shadow: 0 0 1px lawngreen;\n"
                                    "    }\n"
                                    "\n"
                                    "    input:checked + .toggle-slider:before {\n"
                                    "        transform: translateX(26px);\n"
                                    "    }\n"
                                    "\n"
                                    "    input[type=text] {\n"
                                    "        padding-left: 5px;\n"
                                    "        padding-right: 5px;\n"
                                    "        width: 92%;\n"
                                    "        border: 2px solid black;\n"
                                    "        border-radius: 10px;\n"
                                    "        background-color: #d3d3d3;\n"
                                    "    }\n"
                                    "\n"
                                    "</style>\n"
                                    "<script>\n"
                                    "    function generateModule(name, description) {\n"
                                    "        return {name: name, description: description, selected: false};\n"
                                    "    }\n"
                                    "\n"
                                    "    let modules = [\n"
                                    "        generateModule(\"\", \"\"),\n"
                                    "    ].map((module, index) => {\n"
                                    "        if (index === 0) module.selected = true;\n"
                                    "        return module;\n"
                                    "    });\n"
                                    "\n"
                                    "\n"
                                    "    function renderModules() {\n"
                                    "        // Remove the current ones, this is done one extra time during the start, as it's already empty. But it's fine...\n"
                                    "        // document.getElementById(\"module-selector\").innerHTML = \"\";\n"
                                    "        // document.getElementById(\"module-page\").innerHTML = \"\";\n"
                                    "\n"
                                    "        const module_selector = document.getElementById(\"module-selector\");\n"
                                    "\n"
                                    "        modules.forEach((module, index) => {\n"
                                    "            let moduleElement = document.createElement(\"p\");\n"
                                    "            moduleElement.innerHTML = module.name;\n"
                                    "            moduleElement.onclick = () => {\n"
                                    "                modules = modules.map(module => {\n"
                                    "                    module.selected = false;\n"
                                    "                    return module\n"
                                    "                });\n"
                                    "\n"
                                    "                modules[index].selected = true;\n"
                                    "                renderModules();\n"
                                    "            };\n"
                                    "            moduleElement.className += \"unselectable \";\n"
                                    "\n"
                                    "            if (module.selected)\n"
                                    "                moduleElement.className += \"selected\";\n"
                                    "\n"
                                    "            module_selector.appendChild(moduleElement);\n"
                                    "        });\n"
                                    "\n"
                                    "        const moduleElement = document.getElementById(\"module-page\");\n"
                                    "        const selectedModule = modules.find(module => module.selected);\n"
                                    "        if (selectedModule !== undefined) {\n"
                                    "            // Create the title\n"
                                    "            const moduleTitle = document.createElement(\"p\");\n"
                                    "            moduleTitle.id = \"module-title\";\n"
                                    "            moduleTitle.className = \"unselectable\";\n"
                                    "            moduleTitle.innerHTML = selectedModule.name;\n"
                                    "            moduleElement.appendChild(moduleTitle);\n"
                                    "\n"
                                    "            // Create the horizontal line\n"
                                    "            const moduleHorizontalLine = document.createElement(\"div\");\n"
                                    "            moduleHorizontalLine.className = \"line-horizontal\";\n"
                                    "            moduleElement.appendChild(moduleHorizontalLine);\n"
                                    "\n"
                                    "            // Description\n"
                                    "            const moduleDescription = document.createElement(\"p\");\n"
                                    "            moduleDescription.id = \"module-description\";\n"
                                    "            moduleDescription.className = \"unselectable\";\n"
                                    "            moduleDescription.innerHTML = selectedModule.description;\n"
                                    "            moduleElement.appendChild(moduleDescription);\n"
                                    "\n"
                                    "        }\n"
                                    "    }\n"
                                    "\n"
                                    "    window.onload = () => {\n"
                                    "        renderModules();\n"
                                    "    };\n"
                                    "</script>\n"
                                    "<body style=>\n"
                                    "<div class=\"background\">\n"
                                    "    <div class=\"selector\">\n"
                                    "        <div id=\"module-selector\" class=\"selector-list\">\n"
                                    "            <!--                <p class=\"selected\">Wall FPS</p>-->\n"
                                    "            <!--                <p>Sand FPS</p>-->\n"
                                    "            <!--                <p>TNT FPS</p>-->\n"
                                    "        </div>\n"
                                    "    </div>\n"
                                    "    <div class=\"line-vertical\"></div>\n"
                                    "    <div id=\"module-page\" class=\"module\">\n"
                                    "        <!--            <p id=\"module-title\">Wall FPS</p>-->\n"
                                    "        <!--            <div class=\"line-horizontal\"></div>-->\n"
                                    "        <!--            <p id=\"module-description\">Increases your FPS when near walls a significant amount by  culling unseen blocks.</p>-->\n"
                                    "        <div class=\"module-setting-container\">\n"
                                    "            <div class=\"module-setting\">Slider</div>\n"
                                    "            <div class=\"module-setting\">\n"
                                    "                <div class=\"slider-container\">\n"
                                    "                    <input type=\"range\" min=\"1\" max=\"100\" value=\"50\" class=\"slider\" id=\"slider_val\">\n"
                                    "                </div>\n"
                                    "            </div>\n"
                                    "        </div>\n"
                                    "        <div class=\"module-setting-container\">\n"
                                    "            <div class=\"module-setting\">Toggle</div>\n"
                                    "            <div class=\"module-setting\">\n"
                                    "                <label class=\"switch\">\n"
                                    "                    <input type=\"checkbox\">\n"
                                    "                    <span class=\"toggle-slider\"></span>\n"
                                    "                </label>\n"
                                    "            </div>\n"
                                    "        </div>\n"
                                    "        <div class=\"module-setting-container\">\n"
                                    "            <div class=\"module-setting\">Text Input</div>\n"
                                    "            <div class=\"module-setting\">\n"
                                    "                <input type=\"text\"></input>\n"
                                    "            </div>\n"
                                    "        </div>\n"
                                    "        <div class=\"module-setting-container\">\n"
                                    "            <div class=\"module-setting\">Switch</div>\n"
                                    "            <div class=\"module-setting\">\n"
                                    "                <div class=\"select\">\n"
                                    "                    <select/>\n"
                                    "                    <option value=\"foo\">Foo</option>\n"
                                    "                    <option value=\"bar\">Bar</option>\n"
                                    "                    <option value=\"foobar\">Foobar</option>\n"
                                    "                </div>\n"
                                    "            </div>\n"
                                    "        </div>\n"
                                    "    </div>\n"
                                    "</div>\n"
                                    "</body>\n"
                                    "</html>"};
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

        struct
        {
            std::optional<update_context> update_ctx;
        } _previous;

        glm::ivec2 _size;

        ultralight::Ref<ultralight::Renderer> *_renderer;
    };
}

#endif //LETRIS_USER_INPUT_H
