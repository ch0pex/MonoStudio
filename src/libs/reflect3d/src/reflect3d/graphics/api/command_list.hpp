#pragma once

#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/api/pso.hpp"
#include "reflect3d/graphics/core2/pso_states.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/core2/viewport.hpp"

#include <concepts>

namespace rf3d::gfx {

template<typename T>
concept CommandListLifetime = requires(T& cmd) {
  { cmd.reset() } -> std::convertible_to<T const&>;
  { cmd.begin() } -> std::convertible_to<T const&>;
  { cmd.end() } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListPass = requires(T& cmd) {
  { cmd.begin_pass() } -> std::convertible_to<T const&>;
  { cmd.end_pass() } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListSynchronization = requires(
    T& cmd, //
    ResourceState const new_state,
    archetypes::DedicatedBuffer const& buf //
) {
  { cmd.barrier(buf, new_state) } -> std::convertible_to<T const&>;
  { cmd.flush_barriers() } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListBinding = requires(
    T& cmd, //
    T::pso_type const& pso, //
    archetypes::VertexBuffer const& vb,
    archetypes::IndexBuffer const& ib
) { //
  requires PipelineState<typename T::pso_type>;
  { cmd.bind_pipeline(pso) } -> std::convertible_to<T const&>;
  { cmd.bind_vertex_buffer(vb) } -> std::convertible_to<T const&>;
  { cmd.bind_index_buffer(ib) } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListState = requires(
    T& cmd, //
    Viewport const& viewport, //
    Rect2D const& scissor, //
    PrimitiveTopology const topology
) {
  { cmd.set_viewport(viewport) } -> std::convertible_to<T const&>;
  { cmd.set_scissor(scissor) } -> std::convertible_to<T const&>;
  { cmd.primitive_topology(topology) } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListPushConstants = requires(T& cmd) {
  { cmd.push_constants() } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListTransfer = requires(
    T& cmd, //
    archetypes::SourceBuffer& src_buffer, //
    archetypes::DestinationBuffer& dst_buffer, //
    archetypes::CopySourceTexture& src_texture, //
    archetypes::CopyDestTexture& dst_texture //
) {
  { cmd.copy_buffer(src_buffer, dst_buffer) } -> std::convertible_to<T const&>;
  { cmd.copy_texture(src_texture, dst_texture) } -> std::convertible_to<T const&>;
  { cmd.copy_texture_to_buffer(src_texture, dst_buffer) } -> std::convertible_to<T const&>;
  { cmd.copy_buffer_to_texture(src_buffer, dst_texture) } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListCompute = requires(T& cmd) {
  { cmd.dispatch() } -> std::convertible_to<T const&>;
  { cmd.dispatch_indirect() } -> std::convertible_to<T const&>;
};

template<typename T>
concept CommandListDraw = requires(
    T& cmd,
    std::uint32_t vtx_count, //
    std::uint32_t vtx_offset, //
    std::uint32_t idx_count, //
    std::uint32_t inst_count, //
    std::uint32_t first_vtx, //
    std::uint32_t first_idx, //
    std::uint32_t first_inst //
) //
{
  { cmd.draw(vtx_count, inst_count, first_vtx, first_inst) } -> std::convertible_to<T const&>;
  { cmd.draw_indexed(idx_count, inst_count, first_idx, vtx_offset, first_inst) } -> std::convertible_to<T const&>;
  { cmd.draw_indirect() } -> std::convertible_to<T const&>; // TODO: add indirect draw parameters
};

template<typename T>
concept CommandList = //
    std::movable<T> //
    and not std::copyable<T> //
    and CommandListLifetime<T> //
    and CommandListPass<T> //
    and CommandListSynchronization<T> //
    and CommandListBinding<T> //
    and CommandListState<T> //
    and CommandListPushConstants<T> //
    and CommandListTransfer<T> //
    and CommandListCompute<T> //
    and CommandListDraw<T>; //

} // namespace rf3d::gfx
