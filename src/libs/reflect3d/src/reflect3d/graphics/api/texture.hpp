#pragma once

#include "reflect3d/graphics/core2/enum_flags.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/core2/texture_info.hpp"
#include "reflect3d/window/utils/resolution.hpp"

#include <concepts>

namespace rf3d {


template<typename T>
concept Texture = requires(T& texture, typename T::config_type const& config, typename T::state_type state) {
  requires std::movable<T>;
  requires not std::copyable<T>;

  requires std::same_as<typename T::config_type, TextureInfo>;
  requires std::same_as<typename T::usage::value_type, TextureUsage>;
  requires std::same_as<typename T::state_type, ResourceState>;

  typename T::handle_type;
  typename T::view_type;

  { T(config) };
  { texture.view() } -> std::convertible_to<typename T::view_type const&>;
  { texture.resolution() } -> std::same_as<Resolution>;
  { texture.current_state() } -> std::same_as<typename T::state_type>;
  // { texture.set_state(state) } -> std::same_as<void>;
};

template<typename T>
concept ShaderResourceTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::shader_resource>;

template<typename T>
concept RenderTargetTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::render_target>;

template<typename T>
concept DepthStencilTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::depth_stencil>;

template<typename T>
concept CopySourceTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::copy_source>;

template<typename T>
concept CopyDestTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::copy_dest>;

template<typename T>
concept UnorderedAccessTexture = Texture<T> and detail::ContainsFlag<T::usage::value, TextureUsage::unordered_access>;

} // namespace rf3d
