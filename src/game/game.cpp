#include "game.h"

let::game::game(let::network::game *game_network, let::window *window) : _game_network(game_network), _window(window) {

}

void let::game::start() {

}

let::game_builder &let::game_builder::with_network(let::network::game &game_network) {
    _game_network = &game_network;
    return *this;
}

let::game let::game_builder::build() const {
    return let::game(_game_network, _window);
}

let::game_builder &let::game_builder::with_window(let::window &window) {
    _window = &window;
    return *this;
}
