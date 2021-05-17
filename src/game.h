#pragma once

#include <entt/entt.hpp>

namespace let
{
  class game
  {
  public:
    game() = default;



  private:

    entt::basic_registry<uint32_t> _entities;
  };
}
