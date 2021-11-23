#include "world.h"

void let::world::process_packets(let::network::byte_buffer &buffer, let::network::byte_buffer &outgoing) {
    ZoneScopedN("world::process_packets");
    outgoing.clear();

    while (buffer.has_left()) {
        const auto header = let::packets::reader::peek_header(buffer);

        switch (header.id.val)
        {
            case 0x0: {
                const auto packet = let::packets::read<packets::state::play>::keep_alive(buffer);

                let::packets::write<packets::state::play>::keep_alive(outgoing, packet.keep_alive_id);
                break;
            }

            case 0x1: {
                const auto packet = let::packets::read<packets::state::play>::join_game(buffer);

                _player = _create_entity_with_id(packet.entity_id);

                const auto client_brand = std::string("Letris 1.8.9");
                auto brand_bytes = std::vector<std::byte>();
                for (auto letter : client_brand)
                    brand_bytes.push_back(std::byte(letter));

                let::packets::write<packets::state::play>::client_settings(outgoing, "en_GB", std::byte(8), std::byte(0), false, 0);
                let::packets::write<packets::state::play>::plugin_message(outgoing, "minecraft:brand", brand_bytes);
                break;
            }

            case 0x2: {
                const auto packet = let::packets::read<packets::state::play>::chat_message(buffer);
                break;
            }

            case 0x3: {
                const auto packet = let::packets::read<packets::state::play>::time_update(buffer);
                break;
            }

            case 0x4: {
                const auto packet = let::packets::read<packets::state::play>::entity_equipment(buffer);
                break;
            }

            case 0x5: {
                const auto packet = let::packets::read<packets::state::play>::spawn_position(buffer);

                _entities.emplace<entity::home>(_player, packet.position);
                break;
            }

            case 0x6: {
                const auto packet = let::packets::read<packets::state::play>::update_health(buffer);
                break;
            }

            case 0x7: {
                const auto packet = let::packets::read<packets::state::play>::respawn(buffer);
                break;
            }

            case 0x8: {
                const auto packet = let::packets::read<packets::state::play>::player_pos_and_look(buffer);

                if (_entities.any_of<entity::connected>(_player))
                {
                    _entities.get<entity::position>(_player).data = glm::vec3(packet.x, packet.y, packet.z);
                }
                else
                {
                    _entities.emplace<entity::connected>(_player);
                    _entities.emplace<entity::position>(_player, glm::vec3(packet.x, packet.y, packet.z));
                }

                break;
            }

            case 0x9: {
                const auto packet = let::packets::read<packets::state::play>::held_item_change(buffer);
                break;
            }

            case 0xA: {
                const auto packet = let::packets::read<packets::state::play>::use_bed(buffer);
                break;
            }

            case 0xB: {
                const auto packet = let::packets::read<packets::state::play>::animation(buffer);
                break;
            }

            case 0xC: {
                const auto packet = let::packets::read<packets::state::play>::spawn_player(buffer);
                break;
            }

            case 0xD: {
                const auto packet = let::packets::read<packets::state::play>::collect_item(buffer);
                break;
            }

            case 0xE: {
                const auto packet = let::packets::read<packets::state::play>::spawn_object(buffer);
                break;
            }

            case 0xF: {
                const auto packet = let::packets::read<packets::state::play>::spawn_mob(buffer);
                break;
            }

            case 0x10: {
                const auto packet = let::packets::read<packets::state::play>::spawn_painting(buffer);
                break;
            }

            case 0x11: {
                const auto packet = let::packets::read<packets::state::play>::spawn_experience_orb(buffer);
                break;
            }

            case 0x12: {
                const auto packet = let::packets::read<packets::state::play>::entity_velocity(buffer);
                break;
            }

            case 0x13: {
                const auto packet = let::packets::read<packets::state::play>::destroy_entities(buffer);
                break;
            }

            case 0x14: {
                const auto packet = let::packets::read<packets::state::play>::entity(buffer);
                break;
            }

            case 0x15: {
                const auto packet = let::packets::read<packets::state::play>::entity_relative_move(buffer);
                break;
            }

            case 0x16: {
                const auto packet = let::packets::read<packets::state::play>::entity_look(buffer);
                break;
            }

            case 0x17: {
                const auto packet = let::packets::read<packets::state::play>::entity_look_and_relative_move(buffer);
                break;
            }

            case 0x18: {
                const auto packet = let::packets::read<packets::state::play>::entity_teleport(buffer);
                break;
            }

            case 0x19: {
                const auto packet = let::packets::read<packets::state::play>::entity_head_look(buffer);
                break;
            }

            case 0x1A: {
                const auto packet = let::packets::read<packets::state::play>::entity_status(buffer);
                break;
            }

            case 0x1B: {
                const auto packet = let::packets::read<packets::state::play>::attach_entity(buffer);
                break;
            }

            case 0x1C: {
                const auto packet = let::packets::read<packets::state::play>::entity_data(buffer);
                break;
            }

            case 0x1D: {
                const auto packet = let::packets::read<packets::state::play>::entity_effect(buffer);
                break;
            }

            case 0x1E: {
                const auto packet = let::packets::read<packets::state::play>::remove_entity_effect(buffer);
                break;
            }

            case 0x1F: {
                const auto packet = let::packets::read<packets::state::play>::set_experience(buffer);
                break;
            }

            case 0x20: {
                const auto packet = let::packets::read<packets::state::play>::entity_properties(buffer);
                break;
            }

            case 0x21: {
                const auto packet = let::packets::read<packets::state::play>::chunk_data(buffer, false);
                break;
            }

            case 0x22: {
                const auto packet = let::packets::read<packets::state::play>::multi_block_change(buffer);
                break;
            }

            case 0x23: {
                const auto packet = let::packets::read<packets::state::play>::block_change(buffer);
                break;
            }

            case 0x24: {
                const auto packet = let::packets::read<packets::state::play>::block_action(buffer);
                break;
            }

            case 0x25: {
                const auto packet = let::packets::read<packets::state::play>::block_break_animation(buffer);
                break;
            }

            case 0x26: {
                auto packet = let::packets::read<packets::state::play>::map_chunk_bulk(buffer);
                for (auto &chunk : packet.chunks)
                    _chunks.insert({let::chunk::key(chunk), std::move(chunk)});
                break;
            }

            case 0x27: {
                const auto packet = let::packets::read<packets::state::play>::explosion(buffer);
                break;
            }

            case 0x28: {
                const auto packet = let::packets::read<packets::state::play>::effect(buffer);
                break;
            }

            case 0x29: {
                const auto packet = let::packets::read<packets::state::play>::sound_effect(buffer);
                break;
            }

            case 0x2A: {
                const auto packet = let::packets::read<packets::state::play>::particle(buffer);
                break;
            }

            case 0x2B: {
                const auto packet = let::packets::read<packets::state::play>::change_game_state(buffer);
                break;
            }

            case 0x2C: {
                const auto packet = let::packets::read<packets::state::play>::spawn_global_entity(buffer);
                break;
            }

            case 0x2D: {
                const auto packet = let::packets::read<packets::state::play>::open_window(buffer);
                break;
            }

            case 0x2E: {
                const auto packet = let::packets::read<packets::state::play>::close_window(buffer);
                break;
            }

            case 0x2F: {
                const auto packet = let::packets::read<packets::state::play>::set_slot(buffer);
                break;
            }

            case 0x30: {
                const auto packet = let::packets::read<packets::state::play>::window_items(buffer);
                break;
            }

            case 0x31: {
                const auto packet = let::packets::read<packets::state::play>::window_property(buffer);
                break;
            }

            case 0x32: {
                const auto packet = let::packets::read<packets::state::play>::confirm_transaction(buffer);
                break;
            }

            case 0x33: {
                const auto packet = let::packets::read<packets::state::play>::update_sign(buffer);
                break;
            }

            case 0x34: {
                const auto packet = let::packets::read<packets::state::play>::map(buffer);
                break;
            }

            case 0x35: {
                const auto packet = let::packets::read<packets::state::play>::update_block_entity(buffer);
                break;
            }

            case 0x36: {
                const auto packet = let::packets::read<packets::state::play>::open_sign_editor(buffer);
                break;
            }

            case 0x37: {
                const auto packet = let::packets::read<packets::state::play>::statistics(buffer);
                break;
            }

            case 0x38: {
                const auto packet = let::packets::read<packets::state::play>::player_list_item(buffer);
                break;
            }

            case 0x39: {
                const auto packet = let::packets::read<packets::state::play>::player_abilities(buffer);
                break;
            }

            case 0x3A: {
                const auto packet = let::packets::read<packets::state::play>::tab_complete(buffer);
                break;
            }

            case 0x3B: {
                const auto packet = let::packets::read<packets::state::play>::scoreboard_objective(buffer);
                break;
            }

            case 0x3C: {
                const auto packet = let::packets::read<packets::state::play>::update_score(buffer);
                break;
            }

            case 0x3D: {
                const auto packet = let::packets::read<packets::state::play>::display_scoreboard(buffer);
                break;
            }

            case 0x3E: {
                const auto packet = let::packets::read<packets::state::play>::teams(buffer);
                break;
            }

            case 0x3F: {
                const auto packet = let::packets::read<packets::state::play>::plugin_message(buffer);
                break;
            }

            case 0x40: {
                const auto packet = let::packets::read<packets::state::play>::disconnect(buffer);
                break;
            }

            case 0x41: {
                const auto packet = let::packets::read<packets::state::play>::server_difficulty(buffer);
                break;
            }

            case 0x42: {
                const auto packet = let::packets::read<packets::state::play>::combat_event(buffer);
                break;
            }

            case 0x43: {
                const auto packet = let::packets::read<packets::state::play>::camera(buffer);
                break;
            }

            case 0x44: {
                const auto packet = let::packets::read<packets::state::play>::world_border(buffer);
                break;
            }

            case 0x45: {
                const auto packet = let::packets::read<packets::state::play>::title(buffer);
                break;
            }

            case 0x46: {
                const auto packet = let::packets::read<packets::state::play>::set_compression(buffer);
                break;
            }

            case 0x47: {
                const auto packet = let::packets::read<packets::state::play>::player_list_header_and_footer(buffer);
                break;
            }

            case 0x48: {
                const auto packet = let::packets::read<packets::state::play>::resource_pack_send(buffer);
                break;
            }

            default:
                LET_EXCEPTION(exception::source_type::network, "Unimplemented packet: {}", header.id.val);
        }
    }



    for (auto player : _entities.view<let::entity::position, let::entity::connected>()) {
        let::packets::write<packets::state::play>::player(outgoing, true);
    }
}

uint64_t let::world::_create_entity_with_id(int server_id) {
    const auto local_id = _entities.create(server_id);
    if (local_id != server_id)
        LET_EXCEPTION(exception::source_type::world, "Failed to create entity with ID: {}, got {}", server_id, local_id);
    return local_id;
}

glm::vec3 let::world::world_pos() const {
    if (_entities.any_of<entity::position>(_player))
        return _entities.get<entity::position>(_player).data;
    else
        return glm::vec3(0, 0, 0);
}
