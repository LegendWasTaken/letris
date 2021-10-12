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

void let::packets::write<let::packets::state::play>::chat_message(let::network::byte_buffer &buffer, const std::string &message) {

    // Calculate size of string
    write_header(buffer, let::var_int(message.size()).length() + message.size() + 1, 0x1);
    let::network::encoder::write(buffer, message);
}

void let::packets::write<let::packets::state::play>::use_entity(let::network::byte_buffer &buffer, let::var_int target, let::var_int type, std::optional<glm::vec3> target_pos) {
    const auto length = 1 + target.length() + type.length() + (target_pos.has_value() ? sizeof(float) * 3 : 0);
    write_header(buffer, length, 0x2);

    let::network::encoder::write(buffer, target);
    let::network::encoder::write(buffer, type);

    if (target_pos.has_value())
    {
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

void let::packets::write<let::packets::state::play>::player_position(let::network::byte_buffer &buffer, double x, double feet_y, double z, bool on_ground) {
    write_header(buffer, 26, 0x4);
    let::network::encoder::write(buffer, x);
    let::network::encoder::write(buffer, feet_y);
    let::network::encoder::write(buffer, z);
    let::network::encoder::write(buffer, on_ground);
}

void let::packets::write<let::packets::state::play>::player_look(let::network::byte_buffer &buffer, float yaw, float pitch, bool on_ground) {
    write_header(buffer, 10, 0x5);
    let::network::encoder::write(buffer, yaw);
    let::network::encoder::write(buffer, pitch);
    let::network::encoder::write(buffer, on_ground);
}

void let::packets::write<let::packets::state::play>::player_position_and_look(let::network::byte_buffer &buffer, double x, double feet_y, double z, float yaw, float pitch, bool on_ground) {
    write_header(buffer, 26, 0x6);
    let::network::encoder::write(buffer, yaw);
    let::network::encoder::write(buffer, pitch);
    let::network::encoder::write(buffer, x);
    let::network::encoder::write(buffer, feet_y);
    let::network::encoder::write(buffer, z);
    let::network::encoder::write(buffer, on_ground);
}

void let::packets::write<let::packets::state::play>::player_digging(let::network::byte_buffer &buffer, std::byte status, glm::ivec3 position, std::byte face) {
    write_header(buffer, 11, 0x7);
    let::network::encoder::write(buffer, status);
    let::network::encoder::write(buffer, position);
    let::network::encoder::write(buffer, face);
}

void let::packets::write<let::packets::state::play>::player_block_placement(let::network::byte_buffer &buffer, glm::ivec3 pos, std::byte face, let::slot item, std::byte cursor_x, std::byte cursor_y, std::byte cursor_z) {
     write_header(buffer, 10, 0x8);
     // God help me when I write this packet oml
     throw std::runtime_error("Unimplemented packet attempted");
}

void let::packets::write<let::packets::state::play>::held_item_change(let::network::byte_buffer &buffer, std::int16_t slot) {
    write_header(buffer, 3, 0x9);
    let::network::encoder::write(buffer, slot);
}

void let::packets::write<let::packets::state::play>::animation(let::network::byte_buffer &buffer) {
    write_header(buffer, 1, 0xa);
}

void let::packets::write<let::packets::state::play>::entity_action(let::network::byte_buffer &buffer, let::var_int entity, let::var_int action, let::var_int action_parameter) {
    write_header(buffer, 1 + entity.length() + action.length() + action_parameter.length(), 0xb);
    let::network::encoder::write(buffer, entity);
    let::network::encoder::write(buffer, action);
    let::network::encoder::write(buffer, action_parameter);
}

void let::packets::write<let::packets::state::play>::steer_vehicle(let::network::byte_buffer &buffer, float sideways, float forward, std::uint8_t flags) {
    write_header(buffer, 10, 0xc);
    let::network::encoder::write(buffer, sideways);
    let::network::encoder::write(buffer, forward);
    let::network::encoder::write(buffer, flags);
}

void let::packets::write<let::packets::state::play>::close_window(let::network::byte_buffer &buffer, std::uint8_t window_id) {
     write_header(buffer, 2, 0xd);
     let::network::encoder::write(buffer, window_id);
}

void let::packets::write<let::packets::state::play>::click_window(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0xe);
    throw std::runtime_error("Unimplemented packet attempted");
}

void let::packets::write<let::packets::state::play>::confirm_transaction(let::network::byte_buffer &buffer, std::byte window_id, std::int16_t action_number, bool accepted) {
    write_header(buffer, 5, 0xf);
    let::network::encoder::write(buffer, window_id);
    let::network::encoder::write(buffer, action_number);
    let::network::encoder::write(buffer, accepted);
}

void let::packets::write<let::packets::state::play>::creative_inventory_action(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0x10);
    throw std::runtime_error("Unimplemented packet attempted");
}

void let::packets::write<let::packets::state::play>::enchant_item(let::network::byte_buffer &buffer, std::byte window_id, std::byte enchantment) {
     write_header(buffer, 3, 0x11);
     let::network::encoder::write(buffer, window_id);
     let::network::encoder::write(buffer, enchantment);
}

void let::packets::write<let::packets::state::play>::update_sign(let::network::byte_buffer &buffer) {
    // write_header(buffer, LENGTH, 0x12);
    throw std::runtime_error("Unimplemented packet attempted");
}

void let::packets::write<let::packets::state::play>::player_abilities(let::network::byte_buffer &buffer, std::byte flags, float flying_speed, float walking_speed) {
    write_header(buffer, 10, 0x13);
    let::network::encoder::write(buffer, flags);
    let::network::encoder::write(buffer, flying_speed);
    let::network::encoder::write(buffer, walking_speed);
}

void let::packets::write<let::packets::state::play>::tab_complete(let::network::byte_buffer &buffer, const std::string &text, bool has_pos, std::optional<glm::ivec3> looking_at_block) {

    const auto length = 2 + let::var_int(text.size()).length() + text.size() + (looking_at_block.has_value() ? 8 : 0);

    write_header(buffer, length, 0x14);
    let::network::encoder::write(buffer, text);
    let::network::encoder::write(buffer, has_pos);
    if (looking_at_block.has_value())
        let::network::encoder::write(buffer, looking_at_block.value());
}

void let::packets::write<let::packets::state::play>::client_settings(let::network::byte_buffer &buffer, const std::string &locale, std::byte view_distance, std::byte chat_mode, bool chat_colours, std::uint8_t skin_parts) {
    const auto length = 5 + let::var_int(locale.size()).length() + locale.size();

    write_header(buffer, length, 0x15);
    let::network::encoder::write(buffer, locale);
    let::network::encoder::write(buffer, view_distance);
    let::network::encoder::write(buffer, chat_mode);
    let::network::encoder::write(buffer, chat_colours);
    let::network::encoder::write(buffer, skin_parts);
}

void let::packets::write<let::packets::state::play>::client_status(let::network::byte_buffer &buffer, let::var_int action) {
     write_header(buffer, 1 + action.length(), 0x16);
     let::network::encoder::write(buffer, action);
}

void let::packets::write<let::packets::state::play>::plugin_message(let::network::byte_buffer &buffer, const std::string &channel, const std::vector<std::byte> &data) {
    write_header(buffer, 1 + let::var_int(channel.size()).length() + channel.size() + data.size(), 0x17);
    let::network::encoder::write(buffer, channel);
    buffer.write_bytes(data.data(), data.size());
}

void let::packets::write<let::packets::state::play>::spectate(let::network::byte_buffer &buffer, let::uuid target_player) {
     write_header(buffer, 17, 0x18);
     let::network::encoder::write(buffer, target_player);
}

void let::packets::write<let::packets::state::play>::resource_pack_status(let::network::byte_buffer &buffer, const std::string &hash, let::var_int result) {
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

    if (packet.data != 0)
    {
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

    for (auto i = 0; i < packet.property_count.val; i++)
    {
        auto property = entity_properties_packet::property();
        let::network::decoder::read(buffer, property.key);
        let::network::decoder::read(buffer, property.value);
        let::network::decoder::read(buffer, property.modifier_count);

        for (auto j = 0; j < property.modifier_count.val; j++)
        {
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
let::packets::read<let::packets::state::play>::chunk_data(let::network::byte_buffer &buffer) {
    auto packet = chunk_data_packet();
    packet.header = read_header(buffer);

    auto x = std::int32_t();
    auto z = std::int32_t();
    auto ground_up = false;
    auto primary_bitmask = std::uint16_t();
    auto chunk_data_size = let::var_int();

    let::network::decoder::read(buffer, x);
    let::network::decoder::read(buffer, z);
    let::network::decoder::read(buffer, ground_up);
    let::network::decoder::read(buffer, primary_bitmask);
    let::network::decoder::read(buffer, chunk_data_size);



    return packet;
}

let::packets::read<let::packets::state::play>::multi_block_change_packet
let::packets::read<let::packets::state::play>::multi_block_change(let::network::byte_buffer &buffer) {
    auto packet = multi_block_change_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::block_change_packet
let::packets::read<let::packets::state::play>::block_change(let::network::byte_buffer &buffer) {
    auto packet = block_change_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::block_action_packet
let::packets::read<let::packets::state::play>::block_action(let::network::byte_buffer &buffer) {
    auto packet = block_action_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::block_break_animation_packet
let::packets::read<let::packets::state::play>::block_break_animation(let::network::byte_buffer &buffer) {
    auto packet = block_break_animation_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::map_chunk_bulk_packet
let::packets::read<let::packets::state::play>::map_chunk_bulk(let::network::byte_buffer &buffer) {
    auto packet = map_chunk_bulk_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::explosion_packet
let::packets::read<let::packets::state::play>::explosion(let::network::byte_buffer &buffer) {
    auto packet = explosion_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::effect_packet
let::packets::read<let::packets::state::play>::effect(let::network::byte_buffer &buffer) {
    auto packet = effect_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::sound_effect_packet
let::packets::read<let::packets::state::play>::sound_effect(let::network::byte_buffer &buffer) {
    auto packet = sound_effect_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::particle_packet
let::packets::read<let::packets::state::play>::particle(let::network::byte_buffer &buffer) {
    auto packet = particle_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::change_game_state_packet
let::packets::read<let::packets::state::play>::change_game_state(let::network::byte_buffer &buffer) {
    auto packet = change_game_state_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::spawn_global_entity_packet
let::packets::read<let::packets::state::play>::spawn_global_entity(let::network::byte_buffer &buffer) {
    auto packet = spawn_global_entity_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::open_window_packet
let::packets::read<let::packets::state::play>::open_window(let::network::byte_buffer &buffer) {
    auto packet = open_window_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::close_window_packet
let::packets::read<let::packets::state::play>::close_window(let::network::byte_buffer &buffer) {
    auto packet = close_window_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::set_slot_packet
let::packets::read<let::packets::state::play>::set_slot(let::network::byte_buffer &buffer) {
    auto packet = set_slot_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::window_items_packet
let::packets::read<let::packets::state::play>::window_items(let::network::byte_buffer &buffer) {
    auto packet = window_items_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::window_property_packet
let::packets::read<let::packets::state::play>::window_property(let::network::byte_buffer &buffer) {
    auto packet = window_property_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::confirm_transaction_packet
let::packets::read<let::packets::state::play>::confirm_transaction(let::network::byte_buffer &buffer) {
    auto packet = confirm_transaction_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::update_sign_packet
let::packets::read<let::packets::state::play>::update_sign(let::network::byte_buffer &buffer) {
    auto packet = update_sign_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::map_packet
let::packets::read<let::packets::state::play>::map(let::network::byte_buffer &buffer) {
    auto packet = map_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::update_block_entity_packet
let::packets::read<let::packets::state::play>::update_block_entity(let::network::byte_buffer &buffer) {
    auto packet = update_block_entity_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::open_sign_editor_packet
let::packets::read<let::packets::state::play>::open_sign_editor(let::network::byte_buffer &buffer) {
    auto packet = open_sign_editor_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::statistics_packet
let::packets::read<let::packets::state::play>::statistics(let::network::byte_buffer &buffer) {
    auto packet = statistics_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::player_list_item_packet
let::packets::read<let::packets::state::play>::player_list_item(let::network::byte_buffer &buffer) {
    auto packet = player_list_item_packet();
    packet.header = read_header(buffer);

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

    return packet;
}

let::packets::read<let::packets::state::play>::scoreboard_objective_packet
let::packets::read<let::packets::state::play>::scoreboard_objective(let::network::byte_buffer &buffer) {
    auto packet = scoreboard_objective_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::update_score_packet
let::packets::read<let::packets::state::play>::update_score(let::network::byte_buffer &buffer) {
    auto packet = update_score_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::display_scoreboard_packet
let::packets::read<let::packets::state::play>::display_scoreboard(let::network::byte_buffer &buffer) {
    auto packet = display_scoreboard_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::teams_packet
let::packets::read<let::packets::state::play>::teams(let::network::byte_buffer &buffer) {
    auto packet = teams_packet();
    packet.header = read_header(buffer);

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

    return packet;
}

let::packets::read<let::packets::state::play>::camera_packet
let::packets::read<let::packets::state::play>::camera(let::network::byte_buffer &buffer) {
    auto packet = camera_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::world_border_packet
let::packets::read<let::packets::state::play>::world_border(let::network::byte_buffer &buffer) {
    auto packet = world_border_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::title_packet
let::packets::read<let::packets::state::play>::title(let::network::byte_buffer &buffer) {
    auto packet = title_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::set_compression_packet
let::packets::read<let::packets::state::play>::set_compression(let::network::byte_buffer &buffer) {
    auto packet = set_compression_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::player_list_header_and_footer_packet
let::packets::read<let::packets::state::play>::player_list_header_and_footer(let::network::byte_buffer &buffer) {
    auto packet = player_list_header_and_footer_packet();
    packet.header = read_header(buffer);

    return packet;
}

let::packets::read<let::packets::state::play>::resource_pack_send_packet
let::packets::read<let::packets::state::play>::resource_pack_send(let::network::byte_buffer &buffer) {
    auto packet = resource_pack_send_packet();
    packet.header = read_header(buffer);

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
