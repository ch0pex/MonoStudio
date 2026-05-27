/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file base_config.hpp
 * @date 26/05/2026
 * @brief Base configuration type definitions
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <concepts>

// --- System ---


namespace mono::config {

struct Tag { };

template<typename T>
  requires(std::same_as<typename T::config_concept, Tag>)
T example() {
  return {};
}

/*
 * All game configs must inherit from config::Rflect
 * and provide an example configuration
 */
template<typename T>
concept Program = std::same_as<typename T::config_concept, Tag> and requires(T t) {
  { example<T>() } -> std::same_as<T>;
};

} // namespace mono::config
