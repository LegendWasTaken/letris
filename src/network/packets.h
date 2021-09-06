#pragma once

#include <string>
#include <numeric>
#include <variant>

#include <network/byte_buffer.h>

#include <network/encoder.h>
#include <network/decoder.h>

#include <world/chunk/chunk.h>

#include <nlohmann/json.hpp>

namespace let::packets {
    enum class state {
        play, status, login, handshaking,
    };

    enum class handshake_state {
        status = 0x01,
        login = 0x02
    };

    class reader {
    public:
        struct packet_header {
            let::var_int length;
            let::var_int id;
        };

        [[nodiscard]] static packet_header peek_header(let::network::byte_buffer &buffer);

        struct incoming_packet {
            packet_header header;
        };
    protected:
        [[nodiscard]] static packet_header read_header(let::network::byte_buffer &buffer);
    };

    template<state State>
    class read : public reader {

    };

    class writer {
    protected:
        static void write_header(let::network::byte_buffer &buffer, std::int32_t length, std::int32_t packet_id);
    };

    template<state State>
    class write : public writer {

    };

    template<>
    class write<state::play> : public writer {

    };

    template<>
    class read<state::play> : public reader {
    public:
        struct keep_alive_packet : public reader::incoming_packet {
            let::var_int keep_alive_id;
        };

        [[nodiscard]] static keep_alive_packet keep_alive(let::network::byte_buffer &buffer);

        struct join_game_packet : public reader::incoming_packet {
            std::int32_t entity_id;
            std::uint8_t gamemode;
            std::byte dimension;
            std::uint8_t difficulty;
            std::uint8_t max_players;
            std::string level_type;
            bool reduced_debug_info;
        };

        [[nodiscard]] static join_game_packet join_game(let::network::byte_buffer &buffer);

        struct chat_message_packet : public reader::incoming_packet {
            let::chat json_data;
            std::byte position;
        };

        [[nodiscard]] static chat_message_packet chat_message(let::network::byte_buffer &buffer);

        struct time_update_packet : public reader::incoming_packet {
            std::int64_t world_age;
            std::int64_t time_of_day;
        };

        [[nodiscard]] time_update_packet time_update(let::network::byte_buffer &buffer);

        struct entity_equipment_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::uint16_t entity_slot;
            let::slot item;
        };

        [[nodiscard]] entity_equipment_packet entity_equipment(let::network::byte_buffer &buffer);

        struct spawn_position_packet : public reader::incoming_packet {
            glm::ivec3 position;
        };

        [[nodiscard]] spawn_position_packet spawn_position(let::network::byte_buffer &buffer);

        struct update_health_packet : public reader::incoming_packet {
            float health;
            let::var_int food;
            float food_saturation;
        };

        [[nodiscard]] update_health_packet update_health(let::network::byte_buffer
                                                         &buffer);

        struct respawn_packet : public reader::incoming_packet {
            std::int32_t dimension;
            std::uint8_t difficulty;
            std::uint8_t gamemode;
            std::string level_type;
        };

        [[nodiscard]] respawn_packet respawn(let::network::byte_buffer
                                             &buffer);

        struct player_pos_and_look_packet : public reader::incoming_packet {
            double x;
            double y;
            double z;
            float yaw;
            float pitch;
            std::byte flags;
        };

        [[nodiscard]] player_pos_and_look_packet player_pos_and_look(let::network::byte_buffer
                                                                     &buffer);

        struct held_item_change_packet : public reader::incoming_packet {
            std::byte slot;
        };

        [[nodiscard]] held_item_change_packet held_item_change(let::network::byte_buffer
                                                               &buffer);

        struct use_bed_packet : public reader::incoming_packet {
            let::var_int entity_id;
            glm::ivec3 position;
        };

        [[nodiscard]] use_bed_packet use_bed(let::network::byte_buffer
                                             &buffer);

        struct animation_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::uint8_t animation_id;
        };

        [[nodiscard]] animation_packet animation(let::network::byte_buffer
                                                 &buffer);

        struct spawn_player_packet : public reader::incoming_packet {
            let::var_int entity_id;
            let::uuid uuid;
            std::int32_t x;
            std::int32_t y;
            std::int32_t z;
            let::angle yaw;
            let::angle pitch;
            std::int16_t current_item;
        };

        [[nodiscard]] spawn_player_packet spawn_player(let::network::byte_buffer
                                                       &buffer);

        struct collect_item_packet : public reader::incoming_packet {
            let::var_int collected_id;
            let::var_int collector_id;
        };

        [[nodiscard]] collect_item_packet collect_item(let::network::byte_buffer
                                                       &buffer);

        struct spawn_object_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte type;
            std::int32_t x;
            std::int32_t y;
            std::int32_t z;
            let::angle pitch;
            let::angle yaw;
            std::int32_t data;
            std::optional<std::int16_t> velocity_x;
            std::optional<std::int16_t> velocity_y;
            std::optional<std::int16_t> velocity_z;
        };

        [[nodiscard]] spawn_object_packet spawn_object(let::network::byte_buffer
                                                       &buffer);

        struct spawn_mob_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::uint8_t type;
            std::int32_t x;
            std::int32_t y;
            std::int32_t z;
            let::angle yaw;
            let::angle pitch;
            let::angle head_pitch;
            std::int16_t velocity_x;
            std::int16_t velocity_y;
            std::int16_t velocity_z;
            let::entity_metadata metadata;
        };

        [[nodiscard]] spawn_mob_packet spawn_mob(let::network::byte_buffer
                                                 &buffer);

        struct spawn_painting_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::string title;
            glm::ivec3 position;
            std::uint8_t direction;
        };

        [[nodiscard]] spawn_painting_packet spawn_painting(let::network::byte_buffer
                                                           &buffer);

        struct spawn_experience_orb_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::int32_t x;
            std::int32_t y;
            std::int32_t z;
            std::int16_t count;
        };

        [[nodiscard]] spawn_experience_orb_packet spawn_experience_orb(let::network::byte_buffer
                                                                       &buffer);

        struct entity_velocity_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::int16_t x;
            std::int16_t y;
            std::int16_t z;
        };

        [[nodiscard]] entity_velocity_packet entity_velocity(let::network::byte_buffer
                                                             &buffer);

        struct destroy_entities_packet : public reader::incoming_packet {
            let::var_int count;
            std::vector<let::var_int> entity_ids;
        };

        [[nodiscard]] destroy_entities_packet destroy_entities(let::network::byte_buffer
                                                               &buffer);

        struct entity_packet : public reader::incoming_packet {
            let::var_int entity_id;
        };

        [[nodiscard]] entity_packet entity(let::network::byte_buffer
                                           &buffer);

        struct entity_relative_move_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte x;
            std::byte y;
            std::byte z;
            bool on_ground;
        };

        [[nodiscard]] entity_relative_move_packet entity_relative_move(let::network::byte_buffer
                                                                       &buffer);

        struct entity_look_packet : public reader::incoming_packet {
            let::var_int entity_id;
            let::angle yaw;
            let::angle pitch;
            bool on_ground;
        };

        [[nodiscard]] entity_look_packet entity_look(let::network::byte_buffer
                                                     &buffer);

        struct entity_look_and_relative_move_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte x;
            std::byte y;
            std::byte z;
            let::angle yaw;
            let::angle pitch;
            bool on_ground;
        };

        [[nodiscard]] entity_look_and_relative_move_packet entity_look_and_relative_move(let::network::byte_buffer
                                                                                         &buffer);

        struct entity_teleport_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::int32_t x;
            std::int32_t y;
            std::int32_t z;
            let::angle yaw;
            let::angle pitch;
            bool on_ground;
        };

        [[nodiscard]] entity_teleport_packet entity_teleport(let::network::byte_buffer
                                                             &buffer);

        struct entity_head_look_packet : public reader::incoming_packet {
            let::var_int entity_id;
            let::angle yaw;
        };

        [[nodiscard]] entity_head_look_packet entity_head_look(let::network::byte_buffer
                                                               &buffer);

        struct entity_status_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte entity_status;
        };

        [[nodiscard]] entity_status_packet entity_status(let::network::byte_buffer
                                                         &buffer);

        struct attach_entity_packet : public reader::incoming_packet {
            std::int32_t entity_id;
            std::int32_t vehicle_id;
            bool leash;
        };

        [[nodiscard]] attach_entity_packet attach_entity(let::network::byte_buffer
                                                         &buffer);

        struct entity_data_packet : public reader::incoming_packet {
            let::var_int entity_id;
            let::entity_metadata data;
        };

        [[nodiscard]] entity_data_packet entity_data(let::network::byte_buffer
                                                     &buffer);

        struct entity_effect_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte effect_id;
            std::byte amplifier;
            let::var_int seconds;
            bool hide_particles;
        };

        [[nodiscard]] entity_effect_packet entity_effect(let::network::byte_buffer
                                                         &buffer);

        struct remove_entity_effect_packet : public reader::incoming_packet {
            let::var_int entity_id;
            std::byte effect_id;
        };

        [[nodiscard]] remove_entity_effect_packet remove_entity_effect(let::network::byte_buffer
                                                                       &buffer);

        struct set_experience_packet : public reader::incoming_packet {
            float experience_bar;
            let::var_int level;
            let::var_int total_experience;
        };

        [[nodiscard]] set_experience_packet set_experience(let::network::byte_buffer
                                                           &buffer);

        struct entity_properties_packet : public reader::incoming_packet {

            // Todo: With entity class, move to there
            struct property {
                std::string key;
                double value;
                let::var_int modifier_count;
                struct modifier {
                    let::uuid uuid;
                    double amount;
                    std::byte op;
                };
                std::vector<modifier> modifiers;
            };

            let::var_int entity_id;
            let::var_int property_count;
            std::vector<property> properties;
        };

        [[nodiscard]] entity_properties_packet entity_properties(let::network::byte_buffer
                                                                 &buffer);

        struct chunk_data_packet : public reader::incoming_packet {
            std::optional<let::world::chunk> chunk;
        };

        [[nodiscard]] chunk_data_packet chunk_data(let::network::byte_buffer
                                                   &buffer);

        /*
        struct _packet : public reader::incoming_packet {
        };
        [[nodiscard]] _packet (let::network::byte_buffer
        &buffer);

        struct _packet : public reader::incoming_packet {
        };
        [[nodiscard]] _packet (let::network::byte_buffer
        &buffer);

        struct _packet : public reader::incoming_packet {
        };
        [[nodiscard]] _packet (let::network::byte_buffer
        &buffer);

        struct _packet : public reader::incoming_packet {
        };
        [[nodiscard]] _packet (let::network::byte_buffer
        &buffer);

        struct _packet : public reader::incoming_packet {
        };
        [[nodiscard]] _packet (let::network::byte_buffer
        &buffer);
*/
    };

    template<>
    class write<state::status> : public writer {
    public:
        static void request(let::network::byte_buffer &buffer);

        static void ping(let::network::byte_buffer &buffer, std::int64_t payload);
    };

    template<>
    class read<state::status> : public reader {
    public:
        struct response_packet : public reader::incoming_packet {
            struct {
                std::string name;
            } version;

            struct {
                std::uint32_t players_max;
                std::uint32_t online;
            } players;

            struct {
                std::string text;
            } description;
        };

        [[nodiscard]] static response_packet response(let::network::byte_buffer &buffer);

        struct pong_packet : public read::incoming_packet {
            std::int64_t payload;
        };

        [[nodiscard]] static pong_packet pong(let::network::byte_buffer &buffer);
    };


    template<>
    class write<state::login> : public writer {
    public:
        static void login_start(let::network::byte_buffer &buffer, const std::string &name);
    };

    template<>
    class read<state::login> : public reader {
    public:
        struct disconnect_packet : public read::incoming_packet {
            let::chat reason;
        };

        [[nodiscard]] static disconnect_packet disconnect(let::network::byte_buffer &buffer);

        struct encryption_request_packet : public read::incoming_packet {
            std::string server_id;
            std::vector<std::byte> public_key;
            std::vector<std::byte> verify_token;
        };
        [[nodiscard]] static encryption_request_packet encryption_request(let::network::byte_buffer &buffer);

        struct login_success_packet : public read::incoming_packet {
            std::string uuid;
            std::string username;
        };

        [[nodiscard]] static login_success_packet login_success(let::network::byte_buffer &buffer);

        struct set_compression_packet : public read::incoming_packet {
            let::var_int threshold;
        };

        [[nodiscard]] static set_compression_packet set_compression(let::network::byte_buffer &buffer);

    };

    template<>
    class write<state::handshaking> : public writer {
    public:

        static void handshake(let::network::byte_buffer &buffer, const handshake_state state, const std::uint16_t port,
                              const std::string &address);

    };
}
