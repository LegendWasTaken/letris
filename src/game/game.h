#ifndef LETRIS_GAME_H
#define LETRIS_GAME_H

#include <network/game_network.h>
#include <window/window.h>
#include <ui/user_input.h>

namespace let {
    class game {
    public:
        game(let::network::game *game_network, let::window *window);

        void start();

    private:
        let::network::game *_game_network;

        let::window *_window;
    };
}

namespace let {
    class game_builder {
    public:
        game_builder() = default;

        [[nodiscard]] game_builder& with_network(let::network::game &game_network);

        [[nodiscard]] game_builder& with_window(let::window &window);

        [[nodiscard]] let::game build() const;

    private:
        let::network::game *_game_network = nullptr;

        let::window *_window = nullptr;

    };
}

#endif //LETRIS_GAME_H
