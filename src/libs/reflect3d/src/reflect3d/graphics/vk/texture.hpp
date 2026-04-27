#pragma once

#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/texture_info.hpp"
#include "reflect3d/graphics/vk/texture_view.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <mono/misc/passkey.hpp>

#include <type_traits>

namespace rf3d::vk {

template<TextureUsage Usage, bool owning = true>
class Texture {
public:
  // --- Type traits ---

  using handle_type = std::conditional_t<owning, detail::raii::Image, detail::core::Image>;
  using view_type   = TextureView;
  using state_type  = ResourceState;
  using config_type = TextureInfo;
  using usage       = std::integral_constant<TextureUsage, Usage>;

  // --- Constructors ---

  // TODO: implement this function
  explicit Texture(config_type const& config);

  Texture(handle_type handle, detail::raii::ImageView&& view, Resolution const& res)
    requires(not owning)
    : image_handle(handle), image_view(std::move(view)), image_resolution(res) { }

  Texture(Texture const&) = delete;

  Texture(Texture&&) = default;

  Texture& operator=(Texture const&) = delete;

  Texture& operator=(Texture&&) = default;

  ~Texture() = default; // TODO: deferred destruction for textures

  // --- Member functions ---

  [[nodiscard]] view_type view() const
    requires(owning)
  {
    return view_type {
      .gpu_handle = *image_handle,
      .view       = *image_view,
    };
  }

  [[nodiscard]] view_type view() const
    requires(not owning)
  {
    return view_type {
      .gpu_handle = image_handle,
      .view       = *image_view,
    };
  }

  [[nodiscard]] Resolution resolution() const {
    return image_resolution; //
  }

  [[nodiscard]] state_type current_state() const {
    return state; //
  }

  /**
   * Only command lists are allowed to change the state of the texture,
   * because they are responsible for synchronizing access to the texture
   * and ensuring that the correct barriers are in place when transitioning between states.
   *
   * @tparam T the type of the command list that is changing the state of the texture
   */
  template<rf3d::CopyCommandList CmdList>
  void set_state(state_type const new_state, mono::PassKey<CmdList> /*key*/) {
    state = new_state;
  }

private:
  handle_type image_handle;
  detail::raii::ImageView image_view;
  Resolution image_resolution;
  state_type state {ResourceState::undefined};
};

// =================================================================
// ==== TYPE ALIASES ===============================================
// =================================================================

using StaticTexture     = Texture<TextureUsage::shader_resource | TextureUsage::copy_dest | TextureUsage::copy_source>;
using ComputeTexture    = Texture<TextureUsage::unordered_access | TextureUsage::shader_resource>;
using BackBufferTexture = Texture<TextureUsage::render_target | TextureUsage::copy_dest, false>;
using ColorTexture      = Texture<TextureUsage::shader_resource | TextureUsage::copy_dest>;
using RenderTargetTexture = Texture<TextureUsage::render_target | TextureUsage::shader_resource>;
using StorageTexture      = Texture<TextureUsage::unordered_access | TextureUsage::shader_resource>;
using DepthTexture        = Texture<TextureUsage::depth_stencil>;
using DepthStencilTexture = Texture<TextureUsage::depth_stencil>;

// --- Deduction Guides ---

Texture(detail::core::Image, detail::raii::ImageView&&, Resolution const&)
    -> Texture<TextureUsage::render_target | TextureUsage::copy_dest, false>;


} // namespace rf3d::vk
