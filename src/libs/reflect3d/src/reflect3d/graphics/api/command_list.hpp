#pragma once

#include "reflect3d/graphics/api/archetypes/buffer.hpp"
#include "reflect3d/graphics/api/archetypes/texture.hpp"
#include "reflect3d/graphics/api/pso.hpp"
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/viewport.hpp"

#include <concepts>

namespace rf3d {

template<typename T>
concept CommandListLifetime = requires(T& cmd) {
  { cmd.reset() } -> std::same_as<T&>;
  { cmd.begin() } -> std::same_as<T&>;
  { cmd.end() } -> std::same_as<T&>;
};

template<typename T>
concept CommandListPass = requires(T& cmd) {
  { cmd.begin_pass() } -> std::same_as<T&>;
  { cmd.end_pass() } -> std::same_as<T&>;
};

template<typename T>
concept CommandListSynchronization = requires(
    T& cmd, //
    ResourceState const new_state, //
    archetypes::DedicatedBuffer& buf, //
    archetypes::AnyTexture& texture //
) {
  { cmd.barrier(buf, new_state) } -> std::same_as<T&>;
  { cmd.barrier(texture, new_state) } -> std::same_as<T&>;
  { cmd.flush_barriers() } -> std::same_as<T&>;
};

template<typename T>
concept CommandListPsoBinding = requires(T& cmd, T::pso_type const& pso) {
  requires PipelineState<typename T::pso_type>;
  { cmd.bind_pipeline(pso) } -> std::same_as<T&>;
};

template<typename T>
concept CommandListBufferBinding = requires(
    T& cmd, //
    archetypes::VertexBuffer const& vb,
    archetypes::IndexBuffer const& ib
) { //
  { cmd.bind_vertex_buffer(vb) } -> std::same_as<T&>;
  { cmd.bind_index_buffer(ib) } -> std::same_as<T&>;
};

template<typename T>
concept CommandListState = requires(
    T& cmd, //
    Viewport const& viewport, //
    Rect2D const& scissor, //
    PrimitiveTopology const topology
) {
  { cmd.set_viewport(viewport) } -> std::same_as<T&>;
  { cmd.set_scissor(scissor) } -> std::same_as<T&>;
  { cmd.primitive_topology(topology) } -> std::same_as<T&>;
  // { cmd.set_depth_bias() };
  // { cmd.set_depth_test() };
  // { cmd.set_stencil_reference() };
};

template<typename T>
concept CommandListPushConstants = requires(T& cmd, mono::span<std::byte const> data) {
  { cmd.push_constants(data) } -> std::same_as<T&>;
};

template<typename T>
concept CommandListBinding = //
    CommandListPsoBinding<T> and //
    CommandListBufferBinding<T>;

template<typename T>
concept CommandListTransfer = requires(
    T& cmd, //
    archetypes::SourceBuffer& src_buffer, //
    archetypes::DestinationBuffer& dst_buffer, //
    archetypes::CopySourceTexture& src_texture, //
    archetypes::CopyDestTexture& dst_texture //
) {
  // { cmd.copy_buffer(src_buffer, dst_buffer) } -> std::same_as<T&>;
  { cmd.copy_texture(src_texture, dst_texture) } -> std::same_as<T&>;
  { cmd.copy_texture_to_buffer(src_texture, dst_buffer) } -> std::same_as<T&>;
  { cmd.copy_buffer_to_texture(src_buffer, dst_texture) } -> std::same_as<T&>;
};

template<typename T>
concept CommandListCompute = requires(T& cmd, math::uvec3 const vec) {
  { cmd.dispatch(vec) } -> std::same_as<T&>;
  // { cmd.dispatch_indirect() } -> std::same_as<T&>;
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
  { cmd.draw(vtx_count, inst_count, first_vtx, first_inst) } -> std::same_as<T&>;
  { cmd.draw_indexed(idx_count, inst_count, first_idx, vtx_offset, first_inst) } -> std::same_as<T&>;
  // { cmd.draw_indirect() } -> std::same_as<T&>; // TODO: add indirect draw parameters
};

template<typename T>
concept CopyCommandList = //
    std::movable<T> //
    and not std::copyable<T> //
    and CommandListLifetime<T> //
    and CommandListSynchronization<T> //
    and CommandListTransfer<T>;

template<typename T>
concept ComputeCommandList = //
    CopyCommandList<T> //
    and CommandListPsoBinding<T> //
    and CommandListPushConstants<T> //
    and CommandListCompute<T>;

template<typename T>
concept GraphicsCommandList = //
    ComputeCommandList<T> //
    and CommandListPass<T> //
    and CommandListBufferBinding<T> //
    and CommandListState<T> //
    and CommandListDraw<T>;

template<typename T>
concept CommandList = //
    GraphicsCommandList<T> //
    or ComputeCommandList<T> //
    or CopyCommandList<T>;

} // namespace rf3d
