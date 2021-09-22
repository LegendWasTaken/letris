#pragma once

#include <vector>
#include <cstddef>

namespace let::network
{
  class byte_buffer
  {
  public:
    byte_buffer() = default;

    explicit byte_buffer(std::size_t initial_size);

    void write_byte(std::byte byte);

    void write_bytes(const std::byte* bytes, size_t count);

    void clear();

    void step_back(size_t count);

    void reserve(size_t size);

    void resize(size_t size);

    [[nodiscard]] void* data();

    [[nodiscard]] std::byte peek_byte();

    [[nodiscard]] std::byte next_byte();

    [[nodiscard]] std::vector<std::byte> next_bytes(std::size_t byte_count);

    [[nodiscard]] std::size_t size() const noexcept;

    [[nodiscard]] std::size_t capacity() const noexcept;

    [[nodiscard]] std::size_t capacity_left() const noexcept;

    [[nodiscard]] bool empty() const noexcept;

  private:

    void _ensure_capacity(std::size_t capacity);

    std::vector<std::byte> _data;
    std::size_t _read_head = 0;

  };
}
