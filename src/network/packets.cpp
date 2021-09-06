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
/*
let::packets::read<let::packets::state::play>::
let::packets::read<let::packets::state::play>::(let::network::byte_buffer &buffer) {
    auto packet = ();
    packet.header = read_header(buffer);

    let::network::decoder::read(buffer, packet.);

    return packet;
}
*/
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
