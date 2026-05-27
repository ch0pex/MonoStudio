/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file as_span.hpp
 * @date 26/05/2026
 * @brief Helper to convert objects to span view
 */

#pragma once

// --- Includes ---
#include "mono/containers/span.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <memory>

// --- System ---


namespace mono {

/**
 * Converts a single object to a span of one element.
 * This is useful for APIs that expect spans, but you have a single object.
 */
template<typename T>
mono::span<T const> as_span(T const& data) {
  return {std::addressof(data), 1U};
}

/*
 * Converts a single object to a writeable span of one element.
 */
template<typename T>
mono::span<T> as_writeable_span(T& data) {
  return {std::addressof(data), 1U};
}

} // namespace mono
