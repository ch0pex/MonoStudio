#pragma once

#include <unordered_map>
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_pso_builder.hpp"

namespace rf3d::gfx::vk {

class PsoCache {
  // public:
  //   using pso_id_type = std::size_t;
  //   using pso_type    = Pipeline;
  //
  //   PsoManager() = default;
  //
  //   pso_type& operator[](FixedFunctionsConfig const& config) {
  //
  //     psos[0] = PipelineBuilder {config}
  //                   .vertex_stage() // TODO
  //                   .fragment_stage() // TODO
  //                   .build(raii::Device {nullptr}); // TODO
  //   }
  //
  //
  //
  // private:
  //   std::unordered_map<pso_id_type, pso_type> psos;
};

} // namespace rf3d::gfx::vk
