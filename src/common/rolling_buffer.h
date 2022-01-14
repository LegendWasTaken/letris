#ifndef LETRIS_ROLLING_BUFFER_H
#define LETRIS_ROLLING_BUFFER_H

#include <array>
#include <cstddef>

namespace let
{
    template <typename T, size_t MaxCount>
    class rolling_buffer
    {
    private:
        uint64_t _size = 0;
        std::array<T, MaxCount> _values {};

    public:
        rolling_buffer() = default;

        [[nodiscard]] size_t max_size() const noexcept { return MaxCount; }

        [[nodiscard]] size_t size() const noexcept { return _size; }

        [[nodiscard]] T operator[](size_t idx) const { return _values[idx]; }

        [[nodiscard]] T operator[](size_t idx) { return _values[idx]; }

        void push_back(T val) {
            if (_size == MaxCount) {
                for (auto i = 0; i < MaxCount - 1; i++)
                    std::swap(_values[i], _values[i + 1]);
                _size -= 1;
            }
            _values[_size++] = val;
        }

    };
}

#endif //LETRIS_ROLLING_BUFFER_H
