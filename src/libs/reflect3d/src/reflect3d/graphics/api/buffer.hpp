/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file buffer.hpp
 * @date 26/05/2026
 * @brief Buffer concepts for GPU buffer resource constraints
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/enum_flags.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <concepts>

// --- System ---


namespace rf3d {

template<typename T>
concept Buffer = requires(T& buffer) {
  requires std::movable<T>;
  requires not std::copyable<T>;

  // requires std::same_as<typename T::config_type, BufferInfo<typename T::value_type>>;
  requires std::same_as<typename T::usage::value_type, BufferUsage>;
  requires std::same_as<typename T::memory::value_type, MemoryProperty>;

  typename T::size_type;
  typename T::handle_type;

  // { T(config) };
  { buffer.size() } -> std::same_as<typename T::size_type>;
  // { buffer.size_bytes() } -> std::same_as<typename T::size_type>; TODO:
  { buffer.handle() } -> std::same_as<typename T::handle_type>;
};

template<typename T>
concept DedicatedBuffer = requires(T& buffer, T::state_type const state) {
  requires Buffer<T>;
  requires detail::ContainsFlag<T::memory::value, MemoryProperty::dedicated>;
  requires std::same_as<typename T::state_type, ResourceState>;
  { buffer.current_state() } -> std::same_as<typename T::state_type>;
  // { buffer.set_state(state) } -> std::same_as<void>;
};

template<typename T>
concept MappedBuffer = requires(T& buf) {
  requires Buffer<T>;
  requires detail::ContainsFlag<T::memory::value, MemoryProperty::mapped>;
  // requires std::ranges::contiguous_range<T>;
  { buf.template mapped_data<int>() } -> std::same_as<mono::span<int>>; // mock
};

template<typename T>
concept SourceBuffer = Buffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::source>;

template<typename T>
concept DestinationBuffer = Buffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::destination>;

template<typename T>
concept UniformBuffer = MappedBuffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::uniform>;

template<typename T>
concept StorageBuffer = Buffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::storage>;

template<typename T>
concept VertexBuffer = Buffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::vertex>;

template<typename T>
concept IndexBuffer = Buffer<T> and detail::ContainsFlag<T::usage::value, BufferUsage::index>;

} // namespace rf3d
