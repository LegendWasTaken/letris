#include "packets.h"

let::packets::reader::packet_header let::packets::reader::peek_header(let::network::byte_buffer &buffer) {
    auto header = packet_header();
    const auto count = buffer.size();
    let::network::decoder::read(buffer, header.length);
    let::network::decoder::read(buffer, header.id);
    buffer.step_back(count - buffer.size());
    return header;
}

let::packets::reader::packet_header let::packets::reader::read_header(let::network::byte_buffer &buffer) {
    auto header = packet_header();
    let::network::decoder::read(buffer, header.length);
    let::network::decoder::read(buffer, header.id);
    return header;
}

void let::packets::writer::write_header(
        let::network::byte_buffer &buffer,
        std::int32_t length,
        std::int32_t packet_id) {
    let::network::encoder::write(buffer, let::var_int(length));
    let::network::encoder::write(buffer, let::var_int(packet_id));
}

void let::packets::write<let::packets::state::play>::keep_alive(let::network::byte_buffer &buffer, let::var_int id) {
    write_header(buffer, id.length() + 1, 0x0);
    let::network::encoder::write(buffer, id);
}

void let::packets::write<let::packets::state::play>::chat_message(let::network::byte_buffer &buffer,
                                                                  const std::string &message) {

    // Calculate size of string
    write_header(buffer, let::var_int(message.size()).length() + message.size() + 1, 0x1);
    let::network::encoder::write(buffer, message);
}

void let::packets::write<let::packets::state::play>::use_entity(let::network::byte_buffer &buffer, let::var_int target,
                                                                let::var_int type,
                                                                std::optional<glm::vec3> target_pos) {
    const auto length = 1 + target.length() + type.length() + (target_pos.has_value() ? sizeof(float) * 3 : 0);
    write_header(buffer, length, 0x2);

    let::network::encoder::write(buffer, target);
    let::network::encoder::write(buffer, type);

    if (target_pos.has_value()) {
        const auto pos = target_pos.value();

        let::network::encoder::write(buffer, pos.x);
        let::network::encoder::write(buffer, pos.y);
        let::network::encoder::write(buffer, pos.z);
    }
}

void let::packets::write<let::packets::state::play>::player(let::network::byte_buffer &buffer, bool on_ground) {
    write_header(buffer, 2, 0x3);
    let::network::encoder::write(buffer, on_ground);
}

void let::packets::write<let::packets::state::play>::player_position(let::network::byte_buffer &buffer, double x,
                                                                     double feet_y, double z, bool on_ground) {
    write_header(buffer, 26, 0x4);
    let::network::encoder::write(buffer, x);
    let::network::encoder::write(buffer, feet_y);
    let::network::encoder::write(buffer, z);
    let::network::encoder::write(buffer, on_ground);
}

void
let::packets::write<let::packets::state::play>::player_look(let::network::byte_buffer &buffer, float yaw, float pitch,
                                                            bool on_ground) {
    write_header(buffer, 10, 0x5);
    let::network::encoder::write(buffer, yaw);
    let::network::encoder::write(buffer, pitch);
    let::network::encoder::write(buffer, on_ground);
}

void
let::packets::write<let::packets::state::play>::player_position_and_look(let::network::byte_buffer &buffer, double x,
                                                                         double feet_y, double z, float yaw,
                                                                         float pitch, bool on_ground) {
    write_header(buffer, 34, 0x6);
    let::network::encoder::write(buffer, yaw);
    let::network::encoder::write(buffer, pitch);
    let::network::encoder::write(buffer, x);
    let::network::encoder::write(buffer, feet_y);
    let::network::encoder::write(buffer, z);
    let::network::encoder::write(buffer, on_ground);
}

void let::packets::write<let::packets::state::play>::player_digging(let::network::byte_buffer &buffer, std::byte status,
                                                                    glm::ivec3 position, std::byte face) {
    write_header(buffer, 11, 0x7);
    let::network::encoder::write(buffer, status);
    let::network::encoder::write(buffer, position);
    let::network::encoder::write(buffer, face);
}

void let::packets::write<let::packets::state::play>::player_block_placement(let::network::byte_buffer &buffer,
                                                                            glm::ivec3 pos, std::byte face,
                                                                            let::slot item, std::byte cursor_x,
                                                                            std::byte cursor_y, std::byte cursor_z) {
    write_header(buffer, 10, 0x8);
    // God help me when I write this packet oml
    throw std::runtime_error("Unimplemented packet attempted");
}

void
let::packets::write<let::packets::state::play>::held_item_change(let::network::byte_buffer &buffer, std::int16_t slot) {
    write_header(buffer, 3, 0x9);
    let::network::encoder::write(buffer, slot);
}

void let::packets::write<let::packets::state::play>::animation(let::network::byte_buffer &buffer) {
    write_header(buffer, 1, 0xa);
}

void
let::packets::write<let::packets::state::play>::entity_action(let::network::byte_buffer &buffer, let::var_int entity,
                                                              let::var_int action, let::var_int action_parameter) {
    write_header(buffer, 1 + entity.length() + action.length() + action_parameter.length(), 0xb);
    let::network::encoder::write(buffer, entity);
    let::network::encoder::write(buffer, action);
    let::network::encoder::write(buffer, action_parameter);
}

void let::packets::write<let::packets::state::play>::steer_vehicle(let::network::byte_buffer &buffer, float sideways,
                                                                   float forward, std::uint8_t flags) {
    write_header(buffer, 10, 0xc);
    let::network::encoder::write(buffer, sideways);
    let::network::encoder::write(buffer, forward);
    let::network::encoder::write(buffer, flags);
}

void let::packets::write<let::packets::state::play>::close_window(let::network::byte_buffer &buffer,
                                                                  std::uint8_t window_id) {
    write_header(buffer, 2, 0xd);
    let::network::encoder::write(buffer, window_id);
}

void let::packets::write<let::packets::state::play>::click_window(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0xe);
    LET_EXCEPTION(exception::source_type::network, "Attempted to parse unimplemented packet");
}

void let::packets::write<let::packets::state::play>::confirm_transaction(let::network::byte_buffer &buffer,
                                                                         std::byte window_id,
                                                                         std::int16_t action_number, bool accepted) {
    write_header(buffer, 5, 0xf);
    let::network::encoder::write(buffer, window_id);
    let::network::encoder::write(buffer, action_number);
    let::network::encoder::write(buffer, accepted);
}

void let::packets::write<let::packets::state::play>::creative_inventory_action(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0x10);
    LET_EXCEPTION(exception::source_type::network, "Attempted to parse unimplemented packet");
}

void
let::packets::write<let::packets::state::play>::enchant_item(let::network::byte_buffer &buffer, std::byte window_id,
                                                             std::byte enchantment) {
    write_header(buffer, 3, 0x11);
    let::network::encoder::write(buffer, window_id);
    let::network::encoder::write(buffer, enchantment);
}

void let::packets::write<let::packets::state::play>::update_sign(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0x12);
    LET_EXCEPTION(exception::source_type::network, "Attempted to parse unimplemented packet");
}

void
let::packets::write<let::packets::state::play>::player_abilities(let::network::byte_buffer &buffer, std::byte flags,
                                                                 float flying_speed, float walking_speed) {
    write_header(buffer, 10, 0x13);
    let::network::encoder::write(buffer, flags);
    let::network::encoder::write(buffer, flying_speed);
    let::network::encoder::write(buffer, walking_speed);
}

void
let::packets::write<let::packets::state::play>::tab_complete(let::network::byte_buffer &buffer, const std::string &text,
                                                             bool has_pos, std::optional<glm::ivec3> looking_at_block) {

    const auto length = 2 + let::var_int(text.size()).length() + text.size() + (looking_at_block.has_value() ? 8 : 0);

    write_header(buffer, length, 0x14);
    let::network::encoder::write(buffer, text);
    let::network::encoder::write(buffer, has_pos);
    if (looking_at_block.has_value())
        let::network::encoder::write(buffer, looking_at_block.value());
}

void let::packets::write<let::packets::state::play>::client_settings(let::network::byte_buffer &buffer,
                                                                     const std::string &locale, std::byte view_distance,
                                                                     std::byte chat_mode, bool chat_colours,
                                                                     std::uint8_t skin_parts) {
    const auto length = 5 + let::var_int(locale.size()).length() + locale.size();

    write_header(buffer, length, 0x15);
    let::network::encoder::write(buffer, locale);
    let::network::encoder::write(buffer, view_distance);
    let::network::encoder::write(buffer, chat_mode);
    let::network::encoder::write(buffer, chat_colours);
    let::network::encoder::write(buffer, skin_parts);
}

void
let::packets::write<let::packets::state::play>::client_status(let::network::byte_buffer &buffer, let::var_int action) {
    write_header(buffer, 1 + action.length(), 0x16);
    let::network::encoder::write(buffer, action);
}

void let::packets::write<let::packets::state::play>::plugin_message(let::network::byte_buffer &buffer,
                                                                    const std::string &channel,
                                                                    const std::vector<std::byte> &data) {
    write_header(buffer, 1 + let::var_int(channel.size()).length() + channel.size() + data.size(), 0x17);
    let::network::encoder::write(buffer, channel);
    buffer.write_bytes(data.data(), data.size());
}

void
let::packets::write<let::packets::state::play>::spectate(let::network::byte_buffer &buffer, let::uuid target_player) {
    write_header(buffer, 17, 0x18);
    let::network::encoder::write(buffer, target_player);
}

void let::packets::write<let::packets::state::play>::resource_pack_status(let::network::byte_buffer &buffer,
                                                                          const std::string &hash,
                                                                          let::var_int result) {
    write_header(buffer, 1 + result.length() + let::var_int(hash.size()).length() + hash.size(), 0x18);
    let::network::encoder::write(buffer, hash);
    let::network::encoder::write(buffer, result);
}

let::packets::read<let::packets::state::play>::keep_alive_packet
let::packets::read<let::packets::state::play>::keep_alive(let::network::byte_buffer &buffer) {
    auto keep_alive = keep_alive_packet();
    keep_alive.header = read_header(buffer);

    let::network::decoder::read(buffer, keep_alive.keep_alive_id);

    return keep_alive;
}

let::packets::read<let::packets::state::play>::join_game_packet
let::packets::read<let::packets::state::play>::join_game(let::network::byte_buffer &buffer) {
    auto join_game = join_game_packet();
    join_game.header = read_header(buffer);

    let::network::decoder::read(buffer, join_game.entity_id);
    let::network::decoder::read(buffer, join_game.gamemode);
    let::network::decoder::read(buffer, join_game.dimension);
    let::network::decoder::read(buffer, join_game.difficulty);
    let::network::decoder::read(buffer, join_game.max_players);
    let::network::decoder::read(buffer, join_game.level_type);
    let::network::decoder::read(buffer, join_game.reduced_debug_info);
    return join_game;
}

let::packets::read<let::packets::state::play>::chat_message_packet
let::packets::read<let::packets::state::play>::chat_message(let::network::byte_buffer &buffer) {
    auto chat_message = chat_message_packet();
    chat_message.header = read_header(buffer);

    let::network::decoder::read(buffer, chat_message.json_data);
    let::network::decoder::read(buffer, chat_message.position);

    return chat_message;
}

let::packets::read<let::packets::state::play>::time_update_packet
let::packets::read<let::packets::state::play>::time_update(let::network::byte_buffer &buffer) {
    auto packet = time_update_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.world_age);
    let::network::decoder::read(buffer, packet.time_of_day);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_equipment_packet
let::packets::read<let::packets::state::play>::entity_equipment(let::network::byte_buffer &buffer) {
    auto packet = entity_equipment_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.entity_slot);
    let::network::decoder::read(buffer, packet.item);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_position_packet
let::packets::read<let::packets::state::play>::spawn_position(let::network::byte_buffer &buffer) {
    auto packet = spawn_position_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position);

    return packet;
}

let::packets::read<let::packets::state::play>::update_health_packet
let::packets::read<let::packets::state::play>::update_health(let::network::byte_buffer &buffer) {
    auto packet = update_health_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.health);
    let::network::decoder::read(buffer, packet.food);
    let::network::decoder::read(buffer, packet.food_saturation);

    return packet;
}

let::packets::read<let::packets::state::play>::respawn_packet
let::packets::read<let::packets::state::play>::respawn(let::network::byte_buffer &buffer) {
    auto packet = respawn_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.dimension);
    let::network::decoder::read(buffer, packet.difficulty);
    let::network::decoder::read(buffer, packet.gamemode);
    let::network::decoder::read(buffer, packet.level_type);

    return packet;
}

let::packets::read<let::packets::state::play>::player_pos_and_look_packet
let::packets::read<let::packets::state::play>::player_pos_and_look(let::network::byte_buffer &buffer) {
    auto packet = player_pos_and_look_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.flags);

    return packet;
}

let::packets::read<let::packets::state::play>::held_item_change_packet
let::packets::read<let::packets::state::play>::held_item_change(let::network::byte_buffer &buffer) {
    auto packet = held_item_change_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.slot);

    return packet;
}

let::packets::read<let::packets::state::play>::use_bed_packet
let::packets::read<let::packets::state::play>::use_bed(let::network::byte_buffer &buffer) {
    auto packet = use_bed_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.position);

    return packet;
}

let::packets::read<let::packets::state::play>::animation_packet
let::packets::read<let::packets::state::play>::animation(let::network::byte_buffer &buffer) {
    auto packet = animation_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.animation_id);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_player_packet
let::packets::read<let::packets::state::play>::spawn_player(let::network::byte_buffer &buffer) {
    auto packet = spawn_player_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.uuid);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.current_item);
    let::network::decoder::read(buffer, packet.data);

    return packet;
}

let::packets::read<let::packets::state::play>::collect_item_packet
let::packets::read<let::packets::state::play>::collect_item(let::network::byte_buffer &buffer) {
    auto packet = collect_item_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.collected_id);
    let::network::decoder::read(buffer, packet.collector_id);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_object_packet
let::packets::read<let::packets::state::play>::spawn_object(let::network::byte_buffer &buffer) {
    auto packet = spawn_object_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.type);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.data);

    if (packet.data != 0) {
        // Giving the optionals a value
        packet.velocity_x = 0;
        packet.velocity_y = 0;
        packet.velocity_z = 0;

        let::network::decoder::read(buffer, packet.velocity_x.value());
        let::network::decoder::read(buffer, packet.velocity_y.value());
        let::network::decoder::read(buffer, packet.velocity_z.value());
    }

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_mob_packet
let::packets::read<let::packets::state::play>::spawn_mob(let::network::byte_buffer &buffer) {
    auto packet = spawn_mob_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.type);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.head_pitch);
    let::network::decoder::read(buffer, packet.velocity_x);
    let::network::decoder::read(buffer, packet.velocity_y);
    let::network::decoder::read(buffer, packet.velocity_z);
    let::network::decoder::read(buffer, packet.metadata);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_painting_packet
let::packets::read<let::packets::state::play>::spawn_painting(let::network::byte_buffer &buffer) {
    auto packet = spawn_painting_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.title);
    let::network::decoder::read(buffer, packet.position);
    let::network::decoder::read(buffer, packet.direction);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_experience_orb_packet
let::packets::read<let::packets::state::play>::spawn_experience_orb(let::network::byte_buffer &buffer) {
    auto packet = spawn_experience_orb_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.count);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_velocity_packet
let::packets::read<let::packets::state::play>::entity_velocity(let::network::byte_buffer &buffer) {
    auto packet = entity_velocity_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);

    return packet;
}

let::packets::read<let::packets::state::play>::destroy_entities_packet
let::packets::read<let::packets::state::play>::destroy_entities(let::network::byte_buffer &buffer) {
    auto packet = destroy_entities_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.count);

    packet.entity_ids.resize(packet.count.val);
    for (auto i = 0; i < packet.count.val; i++)
        let::network::decoder::read(buffer, packet.entity_ids[i]);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_packet
let::packets::read<let::packets::state::play>::entity(let::network::byte_buffer &buffer) {
    auto packet = entity_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_relative_move_packet
let::packets::read<let::packets::state::play>::entity_relative_move(let::network::byte_buffer &buffer) {
    auto packet = entity_relative_move_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.on_ground);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_look_packet
let::packets::read<let::packets::state::play>::entity_look(let::network::byte_buffer &buffer) {
    auto packet = entity_look_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.on_ground);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_look_and_relative_move_packet
let::packets::read<let::packets::state::play>::entity_look_and_relative_move(let::network::byte_buffer &buffer) {
    auto packet = entity_look_and_relative_move_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.on_ground);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_teleport_packet
let::packets::read<let::packets::state::play>::entity_teleport(let::network::byte_buffer &buffer) {
    auto packet = entity_teleport_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.y);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, packet.yaw);
    let::network::decoder::read(buffer, packet.pitch);
    let::network::decoder::read(buffer, packet.on_ground);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_head_look_packet
let::packets::read<let::packets::state::play>::entity_head_look(let::network::byte_buffer &buffer) {
    auto packet = entity_head_look_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.yaw);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_status_packet
let::packets::read<let::packets::state::play>::entity_status(let::network::byte_buffer &buffer) {
    auto packet = entity_status_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.entity_status);

    return packet;
}

let::packets::read<let::packets::state::play>::attach_entity_packet
let::packets::read<let::packets::state::play>::attach_entity(let::network::byte_buffer &buffer) {
    auto packet = attach_entity_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.vehicle_id);
    let::network::decoder::read(buffer, packet.leash);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_data_packet
let::packets::read<let::packets::state::play>::entity_data(let::network::byte_buffer &buffer) {
    auto packet = entity_data_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.data);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_effect_packet
let::packets::read<let::packets::state::play>::entity_effect(let::network::byte_buffer &buffer) {
    auto packet = entity_effect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.effect_id);
    let::network::decoder::read(buffer, packet.amplifier);
    let::network::decoder::read(buffer, packet.seconds);
    let::network::decoder::read(buffer, packet.hide_particles);

    return packet;
}

let::packets::read<let::packets::state::play>::remove_entity_effect_packet
let::packets::read<let::packets::state::play>::remove_entity_effect(let::network::byte_buffer &buffer) {
    auto packet = remove_entity_effect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.effect_id);

    return packet;
}

let::packets::read<let::packets::state::play>::set_experience_packet
let::packets::read<let::packets::state::play>::set_experience(let::network::byte_buffer &buffer) {
    auto packet = set_experience_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.experience_bar);
    let::network::decoder::read(buffer, packet.level);
    let::network::decoder::read(buffer, packet.total_experience);

    return packet;
}

let::packets::read<let::packets::state::play>::entity_properties_packet
let::packets::read<let::packets::state::play>::entity_properties(let::network::byte_buffer &buffer) {
    auto packet = entity_properties_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.entity_id);
    let::network::decoder::read(buffer, packet.property_count);

    for (auto i = 0; i < packet.property_count; i++) {
        auto property = entity_properties_packet::property();
        let::network::decoder::read(buffer, property.key);
        let::network::decoder::read(buffer, property.value);
        let::network::decoder::read(buffer, property.modifier_count);

        for (auto j = 0; j < property.modifier_count.val; j++) {
            auto modifier = entity_properties_packet::property::modifier();
            let::network::decoder::read(buffer, modifier.uuid);
            let::network::decoder::read(buffer, modifier.amount);
            let::network::decoder::read(buffer, modifier.op);
            property.modifiers.push_back(modifier);
        }
        packet.properties.push_back(property);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::chunk_data_packet
let::packets::read<let::packets::state::play>::chunk_data(let::network::byte_buffer &buffer, bool in_nether) {
    auto packet = chunk_data_packet();
    packet.header = read_header(buffer);

    auto ground_up = false;
    auto primary_bitmask = std::uint16_t();
    auto chunk_data_size = let::var_int();

    let::network::decoder::read(buffer, packet.x);
    let::network::decoder::read(buffer, packet.z);
    let::network::decoder::read(buffer, ground_up);
    let::network::decoder::read(buffer, primary_bitmask);
    let::network::decoder::read(buffer, chunk_data_size);

    auto chunk = let::chunk(packet.x, packet.z);

    for (auto j = 0; j < 16; j++)
    {
        const auto is_set = primary_bitmask & (1 << j);
        if (is_set)
        {
            auto section = std::make_unique<let::chunk_section>();

            const auto blocks = buffer.next_bytes(2 * 4096); // 2048 shorts for the blocks
            for (auto k = 0; k < 4096; k++)
            {
                const auto block = reinterpret_cast<uint16_t const*>(blocks.data())[k];
                section->blocks[k] = let::block(block);
                for (auto l = 0; l < 6; l++)
                    section->blocks[k].set_visible(static_cast<block::face>(l));
            }

            chunk.section_at(j) = std::move(section);
        }
    }

    for (auto j = 0; j < 16; j++)
    {
        const auto is_set = primary_bitmask & (1 << j);
        if (is_set)
            buffer.next_bytes(2048);
    }

    // Todo: figure this out from the chunk size
    if (!in_nether)
        for (auto j = 0; j < 16; j++)
        {
            const auto is_set = primary_bitmask & (1 << j);
            if (is_set)
                buffer.next_bytes(2048);
        }

    buffer.next_bytes(256);

    return packet;
}

let::packets::read<let::packets::state::play>::multi_block_change_packet
let::packets::read<let::packets::state::play>::multi_block_change(let::network::byte_buffer &buffer) {
    auto packet = multi_block_change_packet();
    packet.header = read_header(buffer);

    auto x = int();
    auto z = int();
    auto block_count = let::var_int();
    let::network::decoder::read(buffer, x);
    let::network::decoder::read(buffer, z);
    let::network::decoder::read(buffer, block_count);

    packet.changed_blocks.reserve(block_count.val);
    for (auto i = 0; i < block_count.val; i++) {
        auto horizontal = std::uint8_t();
        auto y_coordinate = std::uint8_t();
        auto block_id = let::var_int();

        let::network::decoder::read(buffer, horizontal);
        let::network::decoder::read(buffer, y_coordinate);
        let::network::decoder::read(buffer, block_id);

        auto target = multi_block_change_packet::block_target();
        target.position.x = (horizontal & 0xF0) + x * 16;
        target.position.z = (horizontal & 0x0F) + z * 16;
        target.position.y = y_coordinate;

        target.block = block_id.val;
        packet.changed_blocks.push_back(target);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::block_change_packet
let::packets::read<let::packets::state::play>::block_change(let::network::byte_buffer &buffer) {
    auto packet = block_change_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position);

    auto block = let::var_int();
    let::network::decoder::read(buffer, block);
    packet.block = block.val;

    return packet;
}

let::packets::read<let::packets::state::play>::block_action_packet
let::packets::read<let::packets::state::play>::block_action(let::network::byte_buffer &buffer) {
    auto packet = block_action_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.coordinate);
    let::network::decoder::read(buffer, packet.bytes[0]);
    let::network::decoder::read(buffer, packet.bytes[1]);

    auto block_type = let::var_int();
    let::network::decoder::read(buffer, block_type);
    packet.block_type = block_type.val;

    return packet;
}

let::packets::read<let::packets::state::play>::block_break_animation_packet
let::packets::read<let::packets::state::play>::block_break_animation(let::network::byte_buffer &buffer) {
    auto packet = block_break_animation_packet();
    packet.header = read_header(buffer);

    auto id = let::var_int();
    let::network::decoder::read(buffer, id);
    packet.entity_id = id.val;

    let::network::decoder::read(buffer, packet.position);
    let::network::decoder::read(buffer, packet.stage);

    return packet;
}

let::packets::read<let::packets::state::play>::map_chunk_bulk_packet
let::packets::read<let::packets::state::play>::map_chunk_bulk(let::network::byte_buffer &buffer) {
    auto packet = map_chunk_bulk_packet();
    packet.header = read_header(buffer);

    auto skylight = false;
    let::network::decoder::read(buffer, skylight);

    auto chunk_count = let::var_int();
    let::network::decoder::read(buffer, chunk_count);

    struct chunk_entry {
        int32_t x;
        int32_t z;
        uint16_t mask;
    };

    auto entries = std::vector<chunk_entry>();
    for (auto i = 0; i < chunk_count.val; i++)
    {
        auto entry = chunk_entry();

        let::network::decoder::read(buffer, entry.x);
        let::network::decoder::read(buffer, entry.z);
        let::network::decoder::read(buffer, entry.mask);

        entries.push_back(entry);
    }

    for (auto i = 0; i < chunk_count.val; i++)
    {
        const auto entry = entries[i];
        auto chunk = let::chunk(entry.x, entry.z);

        for (auto j = 0; j < 16; j++)
        {
            const auto is_set = entry.mask & (1 << j);
            if (is_set)
            {
                auto section = std::make_unique<let::chunk_section>();

                const auto blocks = buffer.next_bytes(2 * 4096); // 2048 shorts for the blocks
                for (auto k = 0; k < 4096; k++)
                {
                    const auto block = reinterpret_cast<uint16_t const*>(blocks.data())[k];
                    section->blocks[k] = let::block(block);
                }

                chunk.section_at(j) = std::move(section);
            }
        }

        for (auto j = 0; j < 16; j++)
        {
            const auto is_set = entry.mask & (1 << j);
            if (is_set)
                buffer.next_bytes(2048);
        }

        if (skylight)
            for (auto j = 0; j < 16; j++)
            {
                const auto is_set = entry.mask & (1 << j);
                if (is_set)
                    buffer.next_bytes(2048);
            }

        buffer.next_bytes(256);

        packet.chunks.push_back(std::move(chunk));
    }

    return packet;
}

let::packets::read<let::packets::state::play>::explosion_packet
let::packets::read<let::packets::state::play>::explosion(let::network::byte_buffer &buffer) {
    auto packet = explosion_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position.x);
    let::network::decoder::read(buffer, packet.position.y);
    let::network::decoder::read(buffer, packet.position.z);
    let::network::decoder::read(buffer, packet.radius);

    auto count = std::int32_t();
    let::network::decoder::read(buffer, count);

    packet.blocks.reserve(count);
    for (int i = 0; i < count; i++) {
        auto offsets = std::array<std::byte, 3>();

        let::network::decoder::read(buffer, offsets[0]);
        let::network::decoder::read(buffer, offsets[1]);
        let::network::decoder::read(buffer, offsets[2]);

        packet.blocks.emplace_back(
                packet.position[0] + static_cast<std::int8_t>(offsets[0]),
                packet.position[1] + static_cast<std::int8_t>(offsets[1]),
                packet.position[2] + static_cast<std::int8_t>(offsets[2]));
    }

    let::network::decoder::read(buffer, packet.motion.x);
    let::network::decoder::read(buffer, packet.motion.y);
    let::network::decoder::read(buffer, packet.motion.z);

    return packet;
}

let::packets::read<let::packets::state::play>::effect_packet
let::packets::read<let::packets::state::play>::effect(let::network::byte_buffer &buffer) {
    auto packet = effect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.position);
    let::network::decoder::read(buffer, packet.data);
    let::network::decoder::read(buffer, packet.disable_relative_volume);

    return packet;
}

let::packets::read<let::packets::state::play>::sound_effect_packet
let::packets::read<let::packets::state::play>::sound_effect(let::network::byte_buffer &buffer) {
    auto packet = sound_effect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.name);
    let::network::decoder::read(buffer, packet.position.x);
    let::network::decoder::read(buffer, packet.position.y);
    let::network::decoder::read(buffer, packet.position.z);
    let::network::decoder::read(buffer, packet.volume);
    let::network::decoder::read(buffer, packet.pitch);

    return packet;
}

let::packets::read<let::packets::state::play>::particle_packet
let::packets::read<let::packets::state::play>::particle(let::network::byte_buffer &buffer) {
    auto packet = particle_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.long_distance);
    let::network::decoder::read(buffer, packet.position.x);
    let::network::decoder::read(buffer, packet.position.y);
    let::network::decoder::read(buffer, packet.position.z);
    let::network::decoder::read(buffer, packet.offset.x);
    let::network::decoder::read(buffer, packet.offset.y);
    let::network::decoder::read(buffer, packet.offset.z);
    let::network::decoder::read(buffer, packet.particle_data);
    let::network::decoder::read(buffer, packet.count);

    auto data_count = 0;
    if (packet.id == 36)
        data_count = 2;
    else if (packet.id == 37 || packet.id == 38)
        data_count = 1;

    for (int i = 0; i < 2; i++) {
        auto val = 0;
        let::network::decoder::read(buffer, val);
        packet.data[i] = val;
    }

    return packet;
}

let::packets::read<let::packets::state::play>::change_game_state_packet
let::packets::read<let::packets::state::play>::change_game_state(let::network::byte_buffer &buffer) {
    auto packet = change_game_state_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.reason);
    let::network::decoder::read(buffer, packet.value);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_global_entity_packet
let::packets::read<let::packets::state::play>::spawn_global_entity(let::network::byte_buffer &buffer) {
    auto packet = spawn_global_entity_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.type);
    let::network::decoder::read(buffer, packet.position);

    return packet;
}

let::packets::read<let::packets::state::play>::open_window_packet
let::packets::read<let::packets::state::play>::open_window(let::network::byte_buffer &buffer) {
    auto packet = open_window_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.type);
    let::network::decoder::read(buffer, packet.title);
    let::network::decoder::read(buffer, packet.slot_count);

    if (packet.type == "EntityHorse") {
        int entity_id;
        let::network::decoder::read(buffer, entity_id);
        packet.entity_id = entity_id;
    }

    return packet;
}

let::packets::read<let::packets::state::play>::close_window_packet
let::packets::read<let::packets::state::play>::close_window(let::network::byte_buffer &buffer) {
    auto packet = close_window_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);

    return packet;
}

let::packets::read<let::packets::state::play>::set_slot_packet
let::packets::read<let::packets::state::play>::set_slot(let::network::byte_buffer &buffer) {
    auto packet = set_slot_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.slot);
    let::network::decoder::read(buffer, packet.data);

    return packet;
}

let::packets::read<let::packets::state::play>::window_items_packet
let::packets::read<let::packets::state::play>::window_items(let::network::byte_buffer &buffer) {
    auto packet = window_items_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);

    auto count = std::int16_t();
    let::network::decoder::read(buffer, count);
    packet.slots.reserve(count);
    for (int i = 0; i < count; i++) {
        auto slot = let::slot();
        let::network::decoder::read(buffer, slot);
        packet.slots.push_back(std::move(slot));
    }

    return packet;
}

let::packets::read<let::packets::state::play>::window_property_packet
let::packets::read<let::packets::state::play>::window_property(let::network::byte_buffer &buffer) {
    auto packet = window_property_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.property);
    let::network::decoder::read(buffer, packet.value);

    return packet;
}

let::packets::read<let::packets::state::play>::confirm_transaction_packet
let::packets::read<let::packets::state::play>::confirm_transaction(let::network::byte_buffer &buffer) {
    auto packet = confirm_transaction_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.id);
    let::network::decoder::read(buffer, packet.action_number);
    let::network::decoder::read(buffer, packet.accepted);

    return packet;
}

let::packets::read<let::packets::state::play>::update_sign_packet
let::packets::read<let::packets::state::play>::update_sign(let::network::byte_buffer &buffer) {
    auto packet = update_sign_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.location);
    for (auto i = 0; i < 4; i++)
        let::network::decoder::read(buffer, packet.lines[i]);

    return packet;
}

let::packets::read<let::packets::state::play>::map_packet
let::packets::read<let::packets::state::play>::map(let::network::byte_buffer &buffer) {
    auto packet = map_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.item_damage);
    let::network::decoder::read(buffer, packet.scale);

    auto icon_count = let::var_int();
    packet.icons.reserve(icon_count.val);
    for (int i = 0; i < icon_count.val; i++) {
        auto icon = map_packet::icon();
        let::network::decoder::read(buffer, icon.direction_and_type);
        let::network::decoder::read(buffer, icon.x);
        let::network::decoder::read(buffer, icon.z);
        packet.icons.push_back(icon);
    }

    let::network::decoder::read(buffer, packet.columns);

    if (static_cast<std::int8_t>(packet.columns) > 0) {
        auto rows = std::byte();
        auto x = std::byte();
        auto z = std::byte();
        auto length = int();
        auto data = std::vector<std::uint8_t>();

        let::network::decoder::read(buffer, rows);
        let::network::decoder::read(buffer, x);
        let::network::decoder::read(buffer, z);
        let::network::decoder::read(buffer, length);

        data.reserve(length);
        for (int i = 0; i < length; i++) {
            auto byte = std::uint8_t();
            let::network::decoder::read(buffer, byte);
            data.push_back(byte);
        }

        packet.rows = rows;
        packet.x = x;
        packet.z = z;
        packet.length = length;
        packet.data = std::move(data);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::update_block_entity_packet
let::packets::read<let::packets::state::play>::update_block_entity(let::network::byte_buffer &buffer) {
    auto packet = update_block_entity_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position);
    let::network::decoder::read(buffer, packet.action);
    packet.data = let::nbt::node::read_optional(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::open_sign_editor_packet
let::packets::read<let::packets::state::play>::open_sign_editor(let::network::byte_buffer &buffer) {
    auto packet = open_sign_editor_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position);

    return packet;
}

let::packets::read<let::packets::state::play>::statistics_packet
let::packets::read<let::packets::state::play>::statistics(let::network::byte_buffer &buffer) {
    auto packet = statistics_packet();
    packet.header = read_header(buffer);

    auto count = let::var_int();
    let::network::decoder::read(buffer, count);
    packet.statistics.reserve(count.val);
    for (int i = 0; i < count.val; i++) {
        auto stat = statistics_packet::statistic();
        auto val = let::var_int();
        let::network::decoder::read(buffer, stat.name);
        let::network::decoder::read(buffer, val);
        stat.value = val.val;
        packet.statistics.push_back(stat);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::player_list_item_packet
let::packets::read<let::packets::state::play>::player_list_item(let::network::byte_buffer &buffer) {
    auto packet = player_list_item_packet();
    packet.header = read_header(buffer);

    auto action = let::var_int();
    let::network::decoder::read(buffer, action);
    packet.action = action.val;

    auto number_players = let::var_int();
    let::network::decoder::read(buffer, number_players);

    packet.players.reserve(number_players.val);
    for (int i = 0; i < number_players.val; i++) {
        auto player = player_list_item_packet::player_data();

        let::network::decoder::read(buffer, player.name);
        switch (action.val) {
            case 0:  {
                let::network::decoder::read(buffer, player.add_player.name);
                auto number_props = let::var_int();
                let::network::decoder::read(buffer, number_props);

                player.add_player.properties.reserve(number_props.val);
                for (int j = 0; j < number_props.val; j++) {
                    auto property = player_list_item_packet::player_data::property();

                    let::network::decoder::read(buffer, property.name);
                    let::network::decoder::read(buffer, property.value);
                    let::network::decoder::read(buffer, property.is_signed);

                    if (property.is_signed) {
                        auto signature = std::string();
                        let::network::decoder::read(buffer, signature);
                        property.signature = std::move(signature);
                    }

                    player.add_player.properties.push_back(std::move(property));
                }

                auto gamemode = let::var_int();
                auto ping = let::var_int();
                let::network::decoder::read(buffer, gamemode);
                let::network::decoder::read(buffer, ping);
                player.add_player.gamemode = gamemode.val;
                player.add_player.ping = ping.val;

                auto has_display_name = bool();
                let::network::decoder::read(buffer, has_display_name);
                if (has_display_name) {
                    auto display_name = let::chat();
                    let::network::decoder::read(buffer, display_name);
                    player.add_player.display_name = std::move(display_name);
                }
                break;
            }
            case 1: {
                auto gamemode = let::var_int();
                let::network::decoder::read(buffer, gamemode);
                player.update_gamemode.gamemode = gamemode.val;
                break;
            }
            case 2: {
                auto ping = let::var_int();
                let::network::decoder::read(buffer, ping);
                player.update_latency.ping = ping.val;
                break;
            }
            case 3: {
                auto has_display_name = bool();
                let::network::decoder::read(buffer, has_display_name);
                if (has_display_name) {
                    auto display_name = let::chat();
                    let::network::decoder::read(buffer, display_name);
                    player.update_display_name.display_name = std::move(display_name);
                }
                break;
            }
        }
    }

    return packet;
}

let::packets::read<let::packets::state::play>::player_abilities_packet
let::packets::read<let::packets::state::play>::player_abilities(let::network::byte_buffer &buffer) {
    auto packet = player_abilities_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.flags);
    let::network::decoder::read(buffer, packet.flying_speed);
    let::network::decoder::read(buffer, packet.walking_speed);

    return packet;
}

let::packets::read<let::packets::state::play>::tab_complete_packet
let::packets::read<let::packets::state::play>::tab_complete(let::network::byte_buffer &buffer) {
    auto packet = tab_complete_packet();
    packet.header = read_header(buffer);

    auto count = let::var_int();
    let::network::decoder::read(buffer, count);
    packet.matches.reserve(count.val);

    for (int i = 0; i < count.val; i++) {
        auto str = std::string();
        let::network::decoder::read(buffer, str);
        packet.matches.push_back(std::move(str));
    }

    return packet;
}

let::packets::read<let::packets::state::play>::scoreboard_objective_packet
let::packets::read<let::packets::state::play>::scoreboard_objective(let::network::byte_buffer &buffer) {
    auto packet = scoreboard_objective_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.name);
    let::network::decoder::read(buffer, packet.mode);

    const auto extra_data = packet.mode == std::byte(0) || packet.mode == std::byte(2);

    if (extra_data) {
        auto value = std::string();
        let::network::decoder::read(buffer, value);
        packet.value = std::move(value);

        auto type = std::string();
        let::network::decoder::read(buffer, type);
        packet.type = std::move(type);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::update_score_packet
let::packets::read<let::packets::state::play>::update_score(let::network::byte_buffer &buffer) {
    auto packet = update_score_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.score_name);
    let::network::decoder::read(buffer, packet.action);
    let::network::decoder::read(buffer, packet.objective_name);

    if (packet.action != std::byte(1)) {
        auto value = let::var_int();
        let::network::decoder::read(buffer, value);
        packet.value = value.val;
    }

    return packet;
}

let::packets::read<let::packets::state::play>::display_scoreboard_packet
let::packets::read<let::packets::state::play>::display_scoreboard(let::network::byte_buffer &buffer) {
    auto packet = display_scoreboard_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.position);
    let::network::decoder::read(buffer, packet.score_name);

    return packet;
}

let::packets::read<let::packets::state::play>::teams_packet
let::packets::read<let::packets::state::play>::teams(let::network::byte_buffer &buffer) {
    auto packet = teams_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.team_name);
    let::network::decoder::read(buffer, packet.mode);

    const auto extra_data = packet.mode == std::byte(0) || packet.mode == std::byte(2);

    if (extra_data) {
        std::string team_display_name;
        std::string team_prefix;
        std::string team_suffix;
        std::byte friendly_fire;
        std::string name_tag_visibility;
        std::byte colour;
        std::vector<std::string> players;

        let::network::decoder::read(buffer, team_display_name);
        let::network::decoder::read(buffer, team_prefix);
        let::network::decoder::read(buffer, team_suffix);
        let::network::decoder::read(buffer, friendly_fire);
        let::network::decoder::read(buffer, name_tag_visibility);
        let::network::decoder::read(buffer, colour);

        auto player_count = let::var_int();
        let::network::decoder::read(buffer, player_count);
        players.reserve(player_count.val);

        for (int i = 0; i < player_count.val; i++) {
            auto player_name = std::string();
            let::network::decoder::read(buffer, player_name);
            players.push_back(std::move(player_name));
        }

        packet.team_display_name = team_display_name;
        packet.team_prefix = team_prefix;
        packet.team_suffix = team_suffix;
        packet.friendly_fire = friendly_fire;
        packet.name_tag_visibility = name_tag_visibility;
        packet.colour = colour;
        packet.players = std::move(players);
    }
    return packet;
}

let::packets::read<let::packets::state::play>::plugin_message_packet
let::packets::read<let::packets::state::play>::plugin_message(let::network::byte_buffer &buffer) {
    auto packet = plugin_message_packet();
    packet.header = read_header(buffer);

    // Since protocol doesnt send size of byte array, rip
    const auto size_before = buffer.size();
    let::network::decoder::read(buffer, packet.channel);
    const auto size_after = buffer.size();

    const auto string_size = size_before - size_after; // Calculate the amount of bytes the string took
    const auto data_size = packet.header.length.val - 1 - string_size; // -1 because of packetID

    packet.data = buffer.next_bytes(data_size);

    return packet;
}

let::packets::read<let::packets::state::play>::disconnect_packet
let::packets::read<let::packets::state::play>::disconnect(let::network::byte_buffer &buffer) {
    auto packet = disconnect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.reason);

    return packet;
}

let::packets::read<let::packets::state::play>::server_difficulty_packet
let::packets::read<let::packets::state::play>::server_difficulty(let::network::byte_buffer &buffer) {
    auto packet = server_difficulty_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.difficulty);

    return packet;
}

let::packets::read<let::packets::state::play>::combat_event_packet
let::packets::read<let::packets::state::play>::combat_event(let::network::byte_buffer &buffer) {
    auto packet = combat_event_packet();
    packet.header = read_header(buffer);

    auto event = let::var_int();
    let::network::decoder::read(buffer, event);
    packet.event = event.val;

    if (packet.event == 0) {
        auto duration = let::var_int();
        let::network::decoder::read(buffer, duration);
        packet.duration = duration.val;
    }

    if (packet.event == 2) {
        auto player_id = let::var_int();
        let::network::decoder::read(buffer, player_id);
        packet.player_id = player_id.val;
    }

    if (packet.event == 0 || packet.event == 2) {
        auto entity_id = let::var_int();
        let::network::decoder::read(buffer, entity_id);
        packet.entity_id = entity_id.val;
    }

    // Todo: this might be broken
    // https://wiki.vg/index.php?title=Protocol&oldid=7368#Combat_Event
    let::network::decoder::read(buffer, packet.message);

    return packet;
}

let::packets::read<let::packets::state::play>::camera_packet
let::packets::read<let::packets::state::play>::camera(let::network::byte_buffer &buffer) {
    auto packet = camera_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.camera_id);

    return packet;
}

let::packets::read<let::packets::state::play>::world_border_packet
let::packets::read<let::packets::state::play>::world_border(let::network::byte_buffer &buffer) {
    auto packet = world_border_packet();
    packet.header = read_header(buffer);

    auto action = let::var_int();
    let::network::decoder::read(buffer, action);
    packet.action = action.val;

    switch (packet.action) {
        case 0: {
            let::network::decoder::read(buffer, packet.set_size.radius);
            break;
        }
        case 1: {
            let::network::decoder::read(buffer, packet.lerp_size.old_radius);
            let::network::decoder::read(buffer, packet.lerp_size.new_radius);
            auto speed = let::var_long();
            let::network::decoder::read(buffer, speed);
            packet.lerp_size.speed = speed.val;
            break;
        }
        case 2: {
            let::network::decoder::read(buffer, packet.set_center.x);
            let::network::decoder::read(buffer, packet.set_center.z);
            break;
        }
        case 3: {
            let::network::decoder::read(buffer, packet.initialize.x);
            let::network::decoder::read(buffer, packet.initialize.z);
            let::network::decoder::read(buffer, packet.initialize.old_radius);
            let::network::decoder::read(buffer, packet.initialize.new_radius);

            auto speed = let::var_long();
            auto boundary = let::var_int();
            auto time = let::var_int();
            auto blocks = let::var_int();

            let::network::decoder::read(buffer, speed);
            let::network::decoder::read(buffer, boundary);
            let::network::decoder::read(buffer, time);
            let::network::decoder::read(buffer, blocks);

            packet.initialize.speed = speed.val;
            packet.initialize.portal_teleport_boundary = boundary.val;
            packet.initialize.warning_time = time.val;
            packet.initialize.warning_blocks = blocks.val;
            break;
        }
        case 4: {
            auto time = let::var_int();
            let::network::decoder::read(buffer, time);
            packet.set_warning_time.warning_time = time.val;
            break;
        }
        case 5: {
            auto blocks = let::var_int();
            let::network::decoder::read(buffer, blocks);
            packet.set_warning_blocks.warning_blocks = blocks.val;
            break;
        }
    }


    return packet;
}

let::packets::read<let::packets::state::play>::title_packet
let::packets::read<let::packets::state::play>::title(let::network::byte_buffer &buffer) {
    auto packet = title_packet();
    packet.header = read_header(buffer);

    auto action = let::var_int();
    let::network::decoder::read(buffer, action);
    packet.action = action.val;

    if (packet.action == 0) {
        let::network::decoder::read(buffer, packet.title);
    } else if (packet.action == 1) {
        let::network::decoder::read(buffer, packet.subtitle);
    } else if (packet.action == 2) {
        let::network::decoder::read(buffer, packet.fade_in);
        let::network::decoder::read(buffer, packet.stay);
        let::network::decoder::read(buffer, packet.fade_out);
    }

    return packet;
}

let::packets::read<let::packets::state::play>::set_compression_packet
let::packets::read<let::packets::state::play>::set_compression(let::network::byte_buffer &buffer) {
    auto packet = set_compression_packet();
    packet.header = read_header(buffer);

    auto threshold = let::var_int();
    let::network::decoder::read(buffer, threshold);
    packet.threshold = threshold.val;

    return packet;
}

let::packets::read<let::packets::state::play>::player_list_header_and_footer_packet
let::packets::read<let::packets::state::play>::player_list_header_and_footer(let::network::byte_buffer &buffer) {
    auto packet = player_list_header_and_footer_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.list_header);
    let::network::decoder::read(buffer, packet.list_footer);

    return packet;
}

let::packets::read<let::packets::state::play>::resource_pack_send_packet
let::packets::read<let::packets::state::play>::resource_pack_send(let::network::byte_buffer &buffer) {
    auto packet = resource_pack_send_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.url);
    let::network::decoder::read(buffer, packet.hash);

    return packet;
}

let::packets::read<let::packets::state::play>::update_entity_nbt_packet
let::packets::read<let::packets::state::play>::update_entity_nbt(let::network::byte_buffer &buffer) {
    auto packet = update_entity_nbt_packet();
    packet.header = read_header(buffer);

    LET_EXCEPTION(exception::source_type::network, "Attempted to parse unimplemented packet");
//    let::network::decoder::read(buffer, packet.entity_id);
//    let::network::decoder::read(buffer, packet.tag);

    return packet;
}

void let::packets::write<let::packets::state::status>::request(let::network::byte_buffer &buffer) {
    write_header(buffer, 1, 0x00);
}

void let::packets::write<let::packets::state::status>::ping(let::network::byte_buffer &buffer, std::int64_t payload) {
    write_header(buffer, 9, 0x01);
    let::network::encoder::write(buffer, payload);
}

let::packets::read<let::packets::state::status>::response_packet
let::packets::read<let::packets::state::status>::response(let::network::byte_buffer &buffer) {
    auto response = response_packet();
    response.header = read_header(buffer);

    auto json_str = std::string();
    let::network::decoder::read(buffer, json_str);

    const auto json = nlohmann::json::parse(json_str.begin(), json_str.end());

    json.at("version").at("name").get_to(response.version.name);
    json.at("players").at("max").get_to(response.players.players_max);
    json.at("players").at("online").get_to(response.players.online);
    json.at("description").get_to(response.description.text);

    return response;
}

let::packets::read<let::packets::state::status>::pong_packet
let::packets::read<let::packets::state::status>::pong(let::network::byte_buffer &buffer) {
    auto pong = pong_packet();
    pong.header = read_header(buffer);

    let::network::decoder::read(buffer, pong.payload);
    return pong;
}

void let::packets::write<let::packets::state::login>::login_start(let::network::byte_buffer &buffer,
                                                                  const std::string &name) {

    const auto packet_length = let::var_int(name.size()).length() + name.size() + 1;

    write_header(buffer, packet_length, 0x00);
    let::network::encoder::write(buffer, name);
}

let::packets::read<let::packets::state::login>::disconnect_packet
let::packets::read<let::packets::state::login>::disconnect(let::network::byte_buffer &buffer) {
    auto packet = disconnect_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.reason);

    return packet;
}

let::packets::read<let::packets::state::login>::encryption_request_packet
let::packets::read<let::packets::state::login>::encryption_request(let::network::byte_buffer &buffer) {
    auto packet = encryption_request_packet();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.server_id);
    auto length = let::var_int();

    let::network::decoder::read(buffer, length);
    packet.public_key = buffer.next_bytes(length.val);

    let::network::decoder::read(buffer, length);
    packet.verify_token = buffer.next_bytes(length.val);

    return packet;
}

let::packets::read<let::packets::state::login>::login_success_packet
let::packets::read<let::packets::state::login>::login_success(let::network::byte_buffer &buffer) {
    auto login_success = login_success_packet();
    login_success.header = read_header(buffer);

    let::network::decoder::read(buffer, login_success.uuid);
    let::network::decoder::read(buffer, login_success.username);
    return login_success;
}

let::packets::read<let::packets::state::login>::set_compression_packet
let::packets::read<let::packets::state::login>::set_compression(let::network::byte_buffer &buffer) {
    auto set_compression = set_compression_packet();

    set_compression.header = read_header(buffer);
    let::network::decoder::read(buffer, set_compression.threshold);

    return set_compression;
}

void let::packets::write<let::packets::state::handshaking>::handshake(let::network::byte_buffer &buffer,
                                                                      const handshake_state state,
                                                                      const std::uint16_t port,
                                                                      const std::string &address) {
    const auto protocol_version = let::var_int(47);
    const auto next_state = let::var_int(static_cast<std::int32_t>(state));

    const auto packet_length = protocol_version.length() +
                               let::var_int(address.size()).length() +
                               address.size() +
                               sizeof(std::uint16_t) +
                               next_state.length() + 1;

    write_header(buffer, packet_length, 0x00);
    let::network::encoder::write(buffer, protocol_version);
    let::network::encoder::write(buffer, address);
    let::network::encoder::write(buffer, port);
    let::network::encoder::write(buffer, next_state);
}
