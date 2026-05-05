/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file reference.hpp
 * @date 30/03/2026
 * @brief Ref wrapper
 *
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <memory>
#include <type_traits>

// --- System ---


namespace mono {

template<typename T>
using Ref = std::reference_wrapper<T>;

/**
 * @brief SafePtr is a non-owning pointer wrapper that provides safe access to the underlying object.
 *
 * It is designed to be used in situations where you want to
 * ensure that the pointer is always valid and cannot be null.
 *
 * It can be constructed from any lvalue reference to an object of type T,
 * and it will store the address of that object.
 *
 * @tparam T The type of the object being pointed to. Must be an object-type (not a function or void).
 */
template<typename T>
  requires std::is_object_v<T>
class SafePtr {
public:
  // --- Type traits ---

  using type = T;

  // --- Constructors ---

  template<typename U>
    requires(std::is_convertible_v<U&, T&> and not std::same_as<std::decay_t<U>, SafePtr>)
  constexpr SafePtr(U&& x) noexcept : ptr(std::addressof(std::forward<U>(x))) { }

  constexpr SafePtr(std::nullptr_t) = delete;

  // --- Member fucntions ---
  constexpr T& operator*() const noexcept { return *ptr; }

  constexpr T* operator->() const noexcept { return ptr; }

  constexpr operator T*() const noexcept { return ptr; }

private:
  T* ptr;
};

template<typename T>
SafePtr(T&) -> SafePtr<T>;

} // namespace mono
