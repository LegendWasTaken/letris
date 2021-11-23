#ifndef LETRIS_COMPONENTS_H
#define LETRIS_COMPONENTS_H

#include <glm/glm.hpp>

#include <cstdint>
#include <cstddef>
#include <string>

namespace let::entity {

    struct connected {
    };

    struct position{
        glm::vec3 data;
    };

    struct home {
        glm::vec3 data;
    };

    struct name {
        std::string data;
    };

    struct ping {
        std::uint64_t value;
    };

}

#endif //LETRIS_COMPONENTS_H
