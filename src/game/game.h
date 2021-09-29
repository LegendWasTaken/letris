#ifndef LETRIS_GAME_H
#define LETRIS_GAME_H

#include <network/game_network.h>
#include <window/window.h>
#include <ui/user_input.h>

namespace let {
    class game {
    public:
        game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer);

        void start();

    private:
        let::network::game *_game_network;

        let::window *_window;

        let::user_input_renderer *_ui_renderer;
    };
}

namespace let {
    class game_builder {
    public:
        game_builder() = default;

        [[nodiscard]] game_builder& with_network(let::network::game &game_network);

        [[nodiscard]] game_builder& with_window(let::window &window);

        [[nodiscard]] game_builder& with_ui_renderer(let::user_input_renderer &ui_renderer);

        [[nodiscard]] let::game build() const;

    private:
        let::network::game *_game_network = nullptr;

        let::window *_window = nullptr;

        let::user_input_renderer *_ui_renderer;
    };
}

#endif //LETRIS_GAME_H
