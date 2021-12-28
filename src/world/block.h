#ifndef LETRIS_BLOCK_H
#define LETRIS_BLOCK_H

#include <cstdint>
#include <array>

namespace let {
    class block {
    private:
        constexpr static uint32_t ID_MASK = 0b1111'1111'1111'1111'0000'0000'0000'0000;
        constexpr static uint32_t ID_SHIFT = 16;

        constexpr static uint32_t NORTH_MASK = 0b0000'0000'0000'0000'1000'0000'0000'0000;
        constexpr static uint32_t SOUTH_MASK = 0b0000'0000'0000'0000'0100'0000'0000'0000;
        constexpr static uint32_t WEST_MASK = 0b0000'0000'0000'0000'0010'0000'0000'0000;
        constexpr static uint32_t EAST_MASK = 0b0000'0000'0000'0000'0001'0000'0000'0000;
        constexpr static uint32_t UP_MASK = 0b0000'0000'0000'0000'0000'1000'0000'0000;
        constexpr static uint32_t DOWN_MASK = 0b0000'0000'0000'0000'0000'0100'0000'0000;

        constexpr static auto FACE_MASKS = std::array<int32_t, 6>({
            NORTH_MASK,
            SOUTH_MASK,
            WEST_MASK,
            EAST_MASK,
            UP_MASK,
            DOWN_MASK
        });

    public:
        enum class face {
            north,
            south,
            west,
            east,
            up,
            down,
        };

        block() = default;

        explicit block(uint16_t id) {
            set_id(id);
        }

        void set_id(uint16_t id) {
            _data = (static_cast<uint32_t>(id) << ID_SHIFT) & ID_MASK;
        }

        [[nodiscard]]
        uint16_t id() const noexcept {
            return static_cast<uint16_t>((_data & ID_MASK) >> ID_SHIFT);
        }

        void set_visible(face block_face) noexcept {
            _data |= FACE_MASKS[static_cast<int32_t>(block_face)];
        }

        [[nodiscard]]
        bool visible(face block_face) const noexcept {
            return _data & FACE_MASKS[static_cast<int32_t>(block_face)];
        }

        uint32_t *data() {
            return &_data;
        }

    private:
        uint32_t _data = 0;
    };
}

#endif //LETRIS_BLOCK_H
