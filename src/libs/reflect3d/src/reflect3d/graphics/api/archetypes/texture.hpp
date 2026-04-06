#pragma once

#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/core/texture_info.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <cstdint>
#include <type_traits>

namespace rf3d::archetypes {

struct TextureView {
  std::uintptr_t gpu_handle;
  std::uintptr_t view;
};

/**
 * Texture archetype that models a GPU texture resource.
 *
 * Since C++ do not support universal quantification, this mock type
 * can be used to by concepts to trick this language limitation.
 */
template<TextureUsage Usage>
struct Texture {
  using handle_type = std::uintptr_t;
  using view_type   = TextureView;
  using state_type  = ResourceState;
  using config_type = TextureInfo;
  using usage       = std::integral_constant<TextureUsage, Usage>;

  Texture() = default;

  Texture(config_type const& config);

  Texture(Texture const&) = delete;

  Texture(Texture&&) = default;

  Texture& operator=(Texture const&) = delete;

  Texture& operator=(Texture&&) = default;

  ~Texture() = default;

  [[nodiscard]] handle_type handle() const;

  [[nodiscard]] view_type view() const;

  [[nodiscard]] Resolution resolution() const;

  [[nodiscard]] state_type current_state() const;

  void set_state(state_type state) const;
};

using ColorTexture        = Texture<TextureUsage::shader_resource | TextureUsage::copy_dest>;
using RenderTargetTexture = Texture<TextureUsage::render_target | TextureUsage::shader_resource>;
using DepthTexture        = Texture<TextureUsage::depth_stencil>;
using DepthStencilTexture = Texture<TextureUsage::depth_stencil>;
using StorageTexture      = Texture<TextureUsage::unordered_access | TextureUsage::shader_resource>;
using CopySourceTexture   = Texture<TextureUsage::copy_source>;
using CopyDestTexture     = Texture<TextureUsage::copy_dest>;

using AnyTexture = Texture< //
  TextureUsage::shader_resource //
| TextureUsage::render_target  //
| TextureUsage::depth_stencil  //
| TextureUsage::unordered_access  //
| TextureUsage::copy_source  //
| TextureUsage::copy_dest //
>;

} // namespace rf3d::archetypes
