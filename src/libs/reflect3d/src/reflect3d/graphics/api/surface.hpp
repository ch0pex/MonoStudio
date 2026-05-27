/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file surface.hpp
 * @date 26/05/2026
 * @brief Surface concept for GPU presentation surface constraints
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/primitive_types.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <concepts>

// --- System ---


namespace rf3d {

template<typename T>
concept Surface = requires( //
    T& surface,  //
    typename T::resolution_type const& resolution,  //
    typename T::window_type&& window //
) {
  requires std::movable<T>;
  requires not std::copyable<T>;
  requires std::same_as<typename T::resolution_type, Resolution>;
  requires std::same_as<typename T::window_type, Window>;
  typename T::image_type;

  { T(std::move(window)) };
  { surface.next_image() } -> std::same_as<typename T::image_type>;
  { surface.present() } -> std::same_as<void>;
  { surface.resize(resolution) } -> std::same_as<void>;
};

} // namespace rf3d
