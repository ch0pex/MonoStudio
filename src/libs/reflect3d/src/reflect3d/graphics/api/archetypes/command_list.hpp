#pragma once

#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/pso.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/core2/pso_states.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/core2/viewport.hpp"

#include <cstdint>

namespace rf3d::gfx::archetypes {

/**
 * CommandList archetype that models a GPU command recording interface.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct CommandList {
  using pso_type = PipelineState;

  CommandList() = default;

  CommandList(CommandList const&) = delete;

  CommandList(CommandList&&) = default;

  CommandList& operator=(CommandList const&) = delete;

  CommandList& operator=(CommandList&&) = default;

  ~CommandList() = default;

  // CommandListLifetime
  [[nodiscard]] CommandList const& reset();
  [[nodiscard]] CommandList const& begin();
  [[nodiscard]] CommandList const& end();

  // CommandListPass
  [[nodiscard]] CommandList const& begin_pass();
  [[nodiscard]] CommandList const& end_pass();

  // CommandListSynchronization
  [[nodiscard]] CommandList const& barrier(DedicatedBuffer const& buf, ResourceState new_state);
  [[nodiscard]] CommandList const& flush_barriers();

  // CommandListBinding
  [[nodiscard]] CommandList const& bind_pipeline(pso_type const& pso);
  [[nodiscard]] CommandList const& bind_vertex_buffer(VertexBuffer const& vb);
  [[nodiscard]] CommandList const& bind_index_buffer(IndexBuffer const& ib);

  // CommandListState
  [[nodiscard]] CommandList const& set_viewport(Viewport const& viewport);
  [[nodiscard]] CommandList const& set_scissor(Rect2D const& scissor);
  [[nodiscard]] CommandList const& primitive_topology(PrimitiveTopology topology);

  // CommandListPushConstants
  [[nodiscard]] CommandList const& push_constants();

  // CommandListTransfer
  [[nodiscard]] CommandList const& copy_buffer(SourceBuffer& src, DestinationBuffer& dst);
  [[nodiscard]] CommandList const& copy_texture(CopySourceTexture& src, CopyDestTexture& dst);
  [[nodiscard]] CommandList const& copy_texture_to_buffer(CopySourceTexture& src, DestinationBuffer& dst);
  [[nodiscard]] CommandList const& copy_buffer_to_texture(SourceBuffer& src, CopyDestTexture& dst);

  // CommandListCompute
  [[nodiscard]] CommandList const& dispatch();
  [[nodiscard]] CommandList const& dispatch_indirect();

  // CommandListDraw
  [[nodiscard]] CommandList const& draw(std::uint32_t vtx_count, std::uint32_t inst_count, std::uint32_t first_vtx, std::uint32_t first_inst);
  [[nodiscard]] CommandList const& draw_indexed(std::uint32_t idx_count, std::uint32_t inst_count, std::uint32_t first_idx, std::uint32_t vtx_offset, std::uint32_t first_inst);
  [[nodiscard]] CommandList const& draw_indirect();
};

} // namespace rf3d::gfx::archetypes
