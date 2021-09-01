#include <memory>
#include <cstring>

namespace let {
    enum class bit_endianness {
        little,
        big
    };

    template<bit_endianness endian>
    class byte_buffer {
    private:
        [[nodiscard]] inline std::uint16_t _bswapu16(std::uint16_t val) const noexcept;

        [[nodiscard]] inline std::uint32_t _bswapu32(std::uint32_t val) const noexcept;

        [[nodiscard]] inline std::uint64_t _bswapu64(std::uint64_t val) const noexcept;

    public:
        byte_buffer(void *data, size_t size)
                : _cursor(0), _size(size), _data(std::make_unique<std::byte[]>(size)) {
            std::memcpy(_data.get(), data, _size);
        }

        [[nodiscard]] std::byte *at_and_increment(size_t size) {
            _cursor += size;
            return _data.get() + (_cursor - size);
        }

        void step_back(size_t count) { _cursor -= count; }

        [[nodiscard]] inline size_t size() const noexcept { return _size; }

        [[nodiscard]] inline std::uint8_t read_u8() {
            return static_cast<std::uint8_t>(_data[_cursor++]);
        }

        [[nodiscard]] inline std::int8_t read_i8() { return static_cast<std::int8_t>(read_u8()); }

        [[nodiscard]] inline std::uint16_t read_u16() {
            auto value = std::uint16_t(0);
            std::memcpy(&value, _data.get() + _cursor, 2);
            _cursor += 2;
            return _bswapu16(value);
        }

        [[nodiscard]] inline std::int16_t read_i16() {
            return static_cast<std::int16_t>(read_u16());
        }

        [[nodiscard]] inline std::uint32_t read_u32() {
            auto value = std::uint32_t(0);
            std::memcpy(&value, _data.get() + _cursor, 4);
            _cursor += 4;
            return _bswapu32(value);
        }

        [[nodiscard]] inline std::int32_t read_i32() {
            return static_cast<std::int32_t>(read_u32());
        }

        [[nodiscard]] inline std::uint64_t read_u64() {
            auto value = std::uint64_t(0);
            std::memcpy(&value, _data.get() + _cursor, 8);
            _cursor += 8;
            return _bswapu64(value);
        }

        [[nodiscard]] inline std::int64_t read_i64() {
            return static_cast<std::int64_t>(read_u64());
        }

        [[nodiscard]] inline float read_f32() {
            const auto as_int = read_u32();
            float ret;
            std::memcpy(&ret, &as_int, 4);
            return ret;
        }

        [[nodiscard]] inline double read_f64() {
            const auto as_long = read_u64();
            double ret;
            std::memcpy(&ret, &as_long, 8);
            return ret;
        }

        [[nodiscard]] inline std::int32_t read_var_int() {
            auto num_read = std::int32_t(0);
            auto result = std::int32_t(0);
            auto read = std::int8_t(0);
            do {
                read = read_u8();
                int value = (read & 0b01111111);
                result |= (value << (7 * num_read));

                num_read++;
                if (num_read > 5) { std::terminate(); }
            } while ((read & 0b10000000) != 0);

            return result;
        }

        [[nodiscard]] inline std::int64_t read_var_long() {
            auto num_read = std::int32_t(0);
            auto result = std::int64_t(0);
            auto read = std::int8_t(0);
            do {
                read = read_u8();
                int value = (read & 0b01111111);
                result |= (value << (7 * num_read));

                num_read++;
                if (num_read > 10) { std::terminate(); }
            } while ((read & 0b10000000) != 0);

            return result;
        }

        void reset() { _cursor = 0; }

    private:
        size_t _cursor;
        size_t _size;
        std::unique_ptr<std::byte[]> _data;
    };

    template<>
    [[nodiscard]] inline std::uint16_t
    byte_buffer<bit_endianness::little>::_bswapu16(std::uint16_t val) const noexcept {
        return val;
    }

    template<>
    [[nodiscard]] inline std::uint16_t
    byte_buffer<bit_endianness::big>::_bswapu16(std::uint16_t val) const noexcept {
        return (val & 0x00'FF) << 8 | (val & 0xFF'00) >> 8;
    }

    template<>
    [[nodiscard]] inline std::uint32_t
    byte_buffer<bit_endianness::little>::_bswapu32(std::uint32_t val) const noexcept {
        return val;
    }

    template<>
    [[nodiscard]] inline std::uint32_t
    byte_buffer<bit_endianness::big>::_bswapu32(std::uint32_t val) const noexcept {
        return (val & 0x00'00'00'FF) << 24 | (val & 0x00'00'FF'00) << 8 |
               (val & 0x00'FF'00'00) >> 8 | (val & 0xFF'00'00'00) >> 24;
    }

    template<>
    [[nodiscard]] inline std::uint64_t
    byte_buffer<bit_endianness::little>::_bswapu64(std::uint64_t val) const noexcept {
        return val;
    }

    template<>
    [[nodiscard]] inline std::uint64_t
    byte_buffer<bit_endianness::big>::_bswapu64(std::uint64_t val) const noexcept {
        return (val & 0x00'00'00'00'00'00'00'FF) << 56 | (val & 0x00'00'00'00'00'00'FF'00) << 40 |
               (val & 0x00'00'00'00'00'FF'00'00) << 24 | (val & 0x00'00'00'00'FF'00'00'00) << 8 |
               (val & 0x00'00'00'FF'00'00'00'00) >> 8 | (val & 0x00'00'FF'00'00'00'00'00) >> 24 |
               (val & 0x00'FF'00'00'00'00'00'00) >> 40 | (val & 0xFF'00'00'00'00'00'00'00) >> 56;
    }
}