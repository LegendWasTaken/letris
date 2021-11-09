#ifndef LETRIS_WORLD_H
#define LETRIS_WORLD_H

#include <common/network/packets.h>
#include <common/exception.h>

#include <world/entities/components.h>

#include <spdlog/spdlog.h>

#include <tracy/Tracy.hpp>

#include <entt/entt.hpp>

#include <unordered_map>

namespace let {
    class world {
    public:
        world() = default;

        void process_packets(let::network::byte_buffer &buffer, let::network::byte_buffer &outgoing);

    private:
        uint64_t _player;

        [[nodiscard]] uint64_t _create_entity_with_id(int server_id);

        entt::basic_registry<uint64_t> _entities;
    };
}

#endif //LETRIS_WORLD_H
