#include "game.h"

let::game::game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer)
        : _game_network(game_network), _window(window), _ui_renderer(ui_renderer) {

}

void let::game::start() {

}

let::game_builder &let::game_builder::with_network(let::network::game &game_network) {
    _game_network = &game_network;
    return *this;
}

let::game let::game_builder::build() const {
    return let::game(_game_network, _window, _ui_renderer);
}

let::game_builder &let::game_builder::with_window(let::window &window) {
    _window = &window;
    return *this;
}

let::game_builder &let::game_builder::with_ui_renderer(let::user_input_renderer &ui_renderer) {
    _ui_renderer = &ui_renderer;
    return *this;
}
