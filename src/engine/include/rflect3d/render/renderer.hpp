#pragma once

#include "rflect3d/core/logging/logger.hpp"
#include "rflect3d/graphics/hri.hpp"

#include <chrono>
#include <string>

namespace rflect {

template<gfx::HRI Backend>
class Renderer {
public:
  using backend_type = Backend;

  void render() {
    LOG_INFO_LIMIT(std::chrono::seconds {1}, "This is rendering");
    // for (auto surface : hri.surfaces())
    //   render surface ()
  }

private:
  backend_type hri;
};

} // namespace rflect
