#ifndef LETRIS_WORLD_H
#define LETRIS_WORLD_H

#include <common/network/packets.h>
#include <common/exception.h>

#include <spdlog/spdlog.h>

namespace let {
    class world {
    public:
        world() = default;

        void process_packets(let::network::byte_buffer &buffer, let::network::byte_buffer &outgoing);

    private:

    };
}

#endif //LETRIS_WORLD_H
