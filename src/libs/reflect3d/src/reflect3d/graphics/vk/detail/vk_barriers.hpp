#pragma once

#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"


namespace rf3d::vk::detail {

struct BarrierInfo {
  core::PipelineStageFlags2 stage_mask;
  core::AccessFlags2 access_mask;
  core::ImageLayout image_layout;
};

} // namespace rf3d::vk::detail
