#ifndef LETRIS_WORLD_H
#define LETRIS_WORLD_H

#include <common/network/packets.h>
#include <common/exception.h>

#include <world/entities/components.h>

#include <spdlog/spdlog.h>

#include <tracy/Tracy.hpp>

#include <entt/entt.hpp>

#include <unordered_map>
#include <bitset>

namespace let {

    namespace bridge {
        class render_data;
    }

    class world {
    public:
        world() = default;

        void process_packets(let::network::byte_buffer &buffer, let::network::byte_buffer &outgoing);

        [[nodiscard]] glm::vec3 world_pos() const;

    private:
        friend bridge::render_data;

        void _update_chunk_visibility(int32_t x, int32_t z);

        uint64_t _player = {};

        uint64_t _last_position_update = 0;

        [[nodiscard]] uint64_t _create_entity_with_id(int server_id);

        std::unordered_map<std::uint64_t, let::chunk> _chunks;

        entt::basic_registry<uint64_t> _entities;
    };
}

#endif //LETRIS_WORLD_H
