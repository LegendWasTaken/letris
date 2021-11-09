#ifndef LETRIS_COMPONENTS_H
#define LETRIS_COMPONENTS_H

#include <glm/glm.hpp>

#include <cstdint>
#include <cstddef>
#include <string>

namespace let::entity {

    struct connected {
    };

    struct position : glm::vec3 {
    };

    struct home : glm::vec3 {
    };

    struct name : std::string {
    };

    struct ping {
        std::uint64_t value;
    };

}

#endif //LETRIS_COMPONENTS_H
