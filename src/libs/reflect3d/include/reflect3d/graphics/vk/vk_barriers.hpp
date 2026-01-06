#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_image.hpp"


namespace rf3d::gfx::vk {

struct ImageLayoutConfig {
  core::ImageLayout old_layout; //
  core::ImageLayout new_layout; //
  core::AccessFlags2 src_access_mask; //
  core::AccessFlags2 dst_access_mask; //
  core::PipelineStageFlags2 src_stage_mask; //
  core::PipelineStageFlags2 dst_stage_mask; //
};

inline constexpr ImageLayoutConfig rendering_layout {
  .old_layout      = core::ImageLayout::eUndefined,
  .new_layout      = core::ImageLayout::eColorAttachmentOptimal,
  .src_access_mask = {},
  .dst_access_mask = core::AccessFlagBits2::eColorAttachmentWrite,
  .src_stage_mask  = core::PipelineStageFlagBits2::eColorAttachmentOutput,
  .dst_stage_mask  = core::PipelineStageFlagBits2::eColorAttachmentOutput,
};

inline constexpr ImageLayoutConfig present_layout {
  .old_layout      = core::ImageLayout::eColorAttachmentOptimal,
  .new_layout      = core::ImageLayout::ePresentSrcKHR,
  .src_access_mask = core::AccessFlagBits2::eColorAttachmentWrite,
  .dst_access_mask = {},
  .src_stage_mask  = core::PipelineStageFlagBits2::eColorAttachmentOutput,
  .dst_stage_mask  = core::PipelineStageFlagBits2::eBottomOfPipe,
};


inline void
transition_image_layout(raii::CommandBuffer const& buffer, Image const& image, ImageLayoutConfig const& config) {
  Image::barrier_type barrier {
    .srcStageMask        = config.src_stage_mask,
    .srcAccessMask       = config.src_access_mask,
    .dstStageMask        = config.dst_stage_mask,
    .dstAccessMask       = config.dst_access_mask,
    .oldLayout           = config.old_layout,
    .newLayout           = config.new_layout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image               = *image,
    .subresourceRange    = {
         .aspectMask     = core::ImageAspectFlagBits::eColor,
         .baseMipLevel   = 0,
         .levelCount     = 1,
         .baseArrayLayer = 0,
         .layerCount     = 1
    }
  };
  core::DependencyInfo dependency_info = {
    .dependencyFlags         = {},
    .imageMemoryBarrierCount = 1,
    .pImageMemoryBarriers    = &barrier,
  };
  buffer.pipelineBarrier2(dependency_info);
}


} // namespace rf3d::gfx::vk
