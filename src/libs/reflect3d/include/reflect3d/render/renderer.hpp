#pragma once

#include <mono/misc/passkey.hpp>


#include <string>

namespace rf3d::gfx {

template<typename Traits>
class Renderer {
public:
  // --- Type traits ---
  using instance_type = typename Traits::instance_type;

  // --- Constructors ---
  explicit Renderer(instance_type&& instance) : instance(std::move(instance)) { }


private:
  instance_type instance;
};

} // namespace rf3d::gfx
