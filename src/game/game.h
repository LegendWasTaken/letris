#ifndef LETRIS_GAME_H
#define LETRIS_GAME_H

#include <network/game_network.h>
#include <network/query_manager.h>
#include <window/window.h>
#include <ui/user_input.h>
#include <tracy/Tracy.hpp>
#include <common/exception.h>

namespace let {
    class game {
    public:
        game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer, let::network::query *server_querier);

        void start();

    private:
        let::network::game *_game_network;

        let::window *_window;

        let::user_input_renderer *_ui_renderer;

        let::network::query *_server_querier;

        std::string _value = "test";

        struct {
            let::main_menu main;
            let::graphics_menu graphics;
            let::multiplayer_menu multiplayer;
        } _menus;

        // Todo: Consider making some type of RAII classes for all OpenGL stuff
        struct {
            struct {
                GLuint target = ~0;
            } texture;
        } _gpu;

        glm::ivec2 _current_resolution;

        void _initialize_menus();

        void _create_gpu_resources();


    };
}

namespace let {
    class game_builder {
    public:
        game_builder() = default;

        [[nodiscard]] game_builder& with_network(let::network::game &game_network);

        [[nodiscard]] game_builder& with_window(let::window &window);

        [[nodiscard]] game_builder& with_ui_renderer(let::user_input_renderer &ui_renderer);

        [[nodiscard]] game_builder& with_server_querier(let::network::query &server_querier);

        [[nodiscard]] let::game build() const;

    private:
        let::network::game *_game_network = nullptr;

        let::window *_window = nullptr;

        let::user_input_renderer *_ui_renderer = nullptr;

        let::network::query *_server_querier = nullptr;
    };
}

#endif //LETRIS_GAME_H
