#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_logical_device.hpp"

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <cstdint>
#include <functional>
#endif

namespace rf3d::gfx::vk {

class CommandBuffer {
public:
  using native_type = raii::CommandBuffer;
  using begin_info  = core::CommandBufferBeginInfo;
  using render_info = core::RenderingInfo;

  explicit CommandBuffer(native_type&& command_buffer) : cmd_buffer(std::move(command_buffer)) { }

  CommandBuffer const& record( // NOLINT
      begin_info const& begin_info, //
      std::function<void(CommandBuffer const&)> const& record_function //
  ) const {
    cmd_buffer.reset();
    cmd_buffer.begin(begin_info);
    record_function(*this);
    cmd_buffer.end();
    return *this;
  }

  CommandBuffer const& record_rendering( // NOLINT
      render_info const& render_info, //
      std::function<void(CommandBuffer const&)> const& record_function //
  ) const {
    cmd_buffer.beginRendering(render_info);
    record_function(*this);
    cmd_buffer.endRendering();
    return *this;
  }

  CommandBuffer const& bind_pipeline( // NOLINT
      core::PipelineBindPoint const bind_point, //
      core::Pipeline const& pipeline //
  ) const {
    cmd_buffer.bindPipeline(bind_point, pipeline);
    return *this;
  }

  CommandBuffer const& draw( // NOLINT
      std::uint32_t const vertex_count, //
      std::uint32_t const instance_count, //
      std::uint32_t const first_vertex, //
      std::uint32_t const first_instance //
  ) const {
    cmd_buffer.draw(vertex_count, instance_count, first_vertex, first_instance);
    return *this;
  }

  CommandBuffer const& set_viewport( // NOLINT
      std::uint32_t const first_viewport, //
      core::Viewport const& viewport //
  ) const {
    cmd_buffer.setViewport(first_viewport, viewport);
    return *this;
  }

  CommandBuffer const& set_scissor(std::uint32_t const first_scissor, core::Rect2D const& scissor) const { /// NOLINT
    cmd_buffer.setScissor(first_scissor, scissor);
    return *this;
  }

  CommandBuffer const& set_cull_mode(core::CullModeFlags const cull_mode) const { /// NOLINT
    cmd_buffer.setCullMode(cull_mode);
    return *this;
  }

  CommandBuffer const& set_front_face(core::FrontFace const front_face) const { /// NOLINT
    cmd_buffer.setFrontFace(front_face);
    return *this;
  }

  CommandBuffer const& set_primitive_topology(core::PrimitiveTopology const topology) const { /// NOLINT
    cmd_buffer.setPrimitiveTopology(topology);
    return *this;
  }

  CommandBuffer const& set_line_width(float const line_width) const { /// NOLINT
    cmd_buffer.setLineWidth(line_width);
    return *this;
  }

  CommandBuffer const& pipeline_barrier2(core::DependencyInfo const& dependency_info) const { /// NOLINT
    cmd_buffer.pipelineBarrier2(dependency_info);
    return *this;
  }

  native_type const& operator*() const noexcept { return cmd_buffer; }

private:
  native_type cmd_buffer;
};


} // namespace rf3d::gfx::vk
