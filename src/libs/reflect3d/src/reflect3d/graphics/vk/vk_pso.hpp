#pragma once

#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

namespace rf3d::gfx::vk {


struct Pso {
  raii::Pipeline layout;
  raii::PipelineLayout handle;
};

class Pipeline {
public:
  using native_type = raii::Pipeline;
  using layout_type = raii::PipelineLayout;

  Pipeline(layout_type&& layout, native_type&& pipeline) : layout(std::move(layout)), handle(std::move(pipeline)) { }

  [[nodiscard]] native_type const& operator*() const { return handle; }

  [[nodiscard]] layout_type const& get_layout() const { return layout; }

private:
  layout_type layout;
  native_type handle;
};


} // namespace rf3d::gfx::vk
