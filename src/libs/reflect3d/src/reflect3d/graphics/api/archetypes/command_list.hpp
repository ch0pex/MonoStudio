#pragma once

#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/pso.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/core2/pso_states.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/core2/viewport.hpp"

#include <mono/containers/span.hpp>

#include <cstdint>

namespace rf3d::archetypes {

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
  [[nodiscard]] CommandList& reset();
  [[nodiscard]] CommandList& begin();
  [[nodiscard]] CommandList& end();

  // CommandListPass
  [[nodiscard]] CommandList& begin_pass();
  [[nodiscard]] CommandList& end_pass();

  // CommandListSynchronization
  [[nodiscard]] CommandList& barrier(DedicatedBuffer const& buf, ResourceState new_state);
  [[nodiscard]] CommandList& barrier(AnyTexture const& texture, ResourceState new_state);
  [[nodiscard]] CommandList& flush_barriers();

  // CommandListBinding
  [[nodiscard]] CommandList& bind_pipeline(pso_type const& pso);
  [[nodiscard]] CommandList& bind_vertex_buffer(VertexBuffer const& vb);
  [[nodiscard]] CommandList& bind_index_buffer(IndexBuffer const& ib);

  // CommandListState
  [[nodiscard]] CommandList& set_viewport(Viewport const& viewport);
  [[nodiscard]] CommandList& set_scissor(Rect2D const& scissor);
  [[nodiscard]] CommandList& primitive_topology(PrimitiveTopology topology);

  // CommandListPushConstants
  [[nodiscard]] CommandList& push_constants(mono::span<std::byte const> bytes);

  // CommandListTransfer
  [[nodiscard]] CommandList& copy_buffer(SourceBuffer& src, DestinationBuffer& dst);
  [[nodiscard]] CommandList& copy_texture(CopySourceTexture& src, CopyDestTexture& dst);
  [[nodiscard]] CommandList& copy_texture_to_buffer(CopySourceTexture& src, DestinationBuffer& dst);
  [[nodiscard]] CommandList& copy_buffer_to_texture(SourceBuffer& src, CopyDestTexture& dst);

  // CommandListCompute
  [[nodiscard]] CommandList& dispatch(math::uvec3 const vec);
  [[nodiscard]] CommandList& dispatch_indirect();

  // CommandListDraw
  [[nodiscard]] CommandList&
  draw(std::uint32_t vtx_count, std::uint32_t inst_count, std::uint32_t first_vtx, std::uint32_t first_inst);
  [[nodiscard]] CommandList& draw_indirect();
  [[nodiscard]] CommandList& draw_indexed(
      std::uint32_t idx_count, //
      std::uint32_t inst_count, //
      std::uint32_t first_idx, //
      std::uint32_t vtx_offset, //
      std::uint32_t first_inst //
  );
};

} // namespace rf3d::archetypes
