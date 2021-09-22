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

void let::network::byte_buffer::write_bytes(const std::byte* bytes, size_t count)
{
  _ensure_capacity(count);

  for (auto i = 0; i < count; i++)
    _data.push_back(bytes[i]);
}

void let::network::byte_buffer::step_back(size_t count) {
    _read_head -= count;
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

std::byte let::network::byte_buffer::peek_byte() {
    return _data[_read_head];
}

std::byte let::network::byte_buffer::next_byte()
{
  return _data[_read_head++];
}

std::vector<std::byte> let::network::byte_buffer::next_bytes(std::size_t byte_count)
{
  auto result = std::vector<std::byte>(byte_count);

  for (std::size_t i = 0; i < byte_count; i++)
    result[i] = _data[_read_head++];

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

void let::network::byte_buffer::reserve(size_t size) {
    _data.reserve(size);
}


void let::network::byte_buffer::resize(size_t size) {
    _data.resize(size);
}

void *let::network::byte_buffer::data() {
    return _data.data();
}
