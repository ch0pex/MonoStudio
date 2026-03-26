#pragma once


#include <memory>
#include "mono/containers/span.hpp"

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
