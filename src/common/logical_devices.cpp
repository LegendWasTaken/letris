#include "logical_devices.h"

let::logical::keyboard::keyboard() {

}

let::logical::keyboard::state let::logical::keyboard::state_of(let::logical::keyboard::key_code key) const noexcept {
    return _keys[static_cast<size_t>(key)].press_state;
}

int let::logical::keyboard::mods_of(let::logical::keyboard::key_code key) const noexcept {
    return _keys[static_cast<size_t>(key)].mods;
}

std::array<let::logical::keyboard::key_state, static_cast<size_t>(let::logical::keyboard::key_code::max_key)> &
let::logical::keyboard::keys() {
    return _keys;
}

bool let::logical::keyboard::is_key_down(let::logical::keyboard::key_code key) const noexcept {
    return
            _keys[static_cast<int>(key)].press_state == state::held ||
            _keys[static_cast<int>(key)].press_state == state::repeat ||
            _keys[static_cast<int>(key)].press_state == state::pressed;
}

bool let::logical::keyboard::is_key_pressed(let::logical::keyboard::key_code key) const noexcept {
    return
        _keys[static_cast<int>(key)].press_state == state::pressed;
}

bool let::logical::keyboard::is_key_up(let::logical::keyboard::key_code key) const noexcept {
    return _keys[static_cast<int>(key)].press_state == state::released;
}

bool let::logical::keyboard::is_key_held(let::logical::keyboard::key_code key) const noexcept {
    return
        _keys[static_cast<int>(key)].press_state == state::held ||
        _keys[static_cast<int>(key)].press_state == state::repeat;
}

let::logical::mouse::mouse() {
    for (auto &button : _buttons)
        button = button_state::up;
}

void let::logical::mouse::set_position(const glm::ivec2 &pos) {
    _position = pos;
}

std::array<let::logical::mouse::button_state, 3> &let::logical::mouse::buttons() {
    return _buttons;
}

let::logical::mouse::button_state let::logical::mouse::state_of(let::logical::mouse::button target) const noexcept {
    return _buttons[static_cast<int>(target)];
}

glm::ivec2 let::logical::mouse::position() const noexcept {
    return _position;
}

void let::logical::mouse::hide() {
    _hidden = true;
}

void let::logical::mouse::show() {
    _hidden = false;
}

bool let::logical::mouse::hidden() const noexcept {
    return _hidden;
}
