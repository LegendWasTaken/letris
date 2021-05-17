#include "byte_buffer.h"

let::network::byte_buffer::byte_buffer(std::size_t initial_size) : _data(), _read_head(0)
{
  _data.reserve(initial_size);
}

void let::network::byte_buffer::write_byte(std::byte byte)
{
  _ensure_capacity(1);

  _data.push_back(byte);
}

void let::network::byte_buffer::write_bytes(std::span<std::byte> bytes)
{
  _ensure_capacity(bytes.size());

  for (auto val : bytes)
    _data.push_back(val);
}

void let::network::byte_buffer::clear()
{
  _data.clear();
  _read_head = 0;
}

void let::network::byte_buffer::_ensure_capacity(std::size_t capacity)
{
  _data.reserve(_data.capacity() + capacity);
}

std::byte let::network::byte_buffer::next_byte()
{
  return _data[_read_head++];
}

std::vector<std::byte> let::network::byte_buffer::_next_bytes(std::size_t byte_count)
{
  auto result = std::vector<std::byte>(byte_count);

  for (std::size_t i = 0; i < byte_count; i++)
    result.push_back(_data[_read_head++]);

  return std::move(result);
}

std::size_t let::network::byte_buffer::size() const noexcept
{
  return _data.size() - _read_head;
}

std::size_t let::network::byte_buffer::capacity() const noexcept
{
  return _data.capacity();
}

std::size_t let::network::byte_buffer::capacity_left() const noexcept
{
  return _data.capacity() - _data.size();
}

bool let::network::byte_buffer::empty() const noexcept
{
  return _data.size() - 1 == _read_head;
}
