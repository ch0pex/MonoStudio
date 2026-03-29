#pragma once

namespace rf3d::archetypes {

/**
 * Fence archetype that models a GPU synchronization primitive.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Fence {
  Fence() = default;

  Fence(Fence const&) = delete;

  Fence(Fence&&) = default;

  Fence& operator=(Fence const&) = delete;

  Fence& operator=(Fence&&) = default;

  ~Fence() = default;

  void wait();

  void reset();

  bool signaled();
};

} // namespace rf3d::archetypes
