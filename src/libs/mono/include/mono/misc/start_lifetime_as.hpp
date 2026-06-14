/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file start_lifetime_as.hpp
 * @date 26/05/2026
 * @brief std::start_lifetime_as utilities for reinterpreting memory
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

#include <cstring>
#include <memory>
#include <type_traits>

// --- System ---


namespace mono {

#if __cpp_lib_start_lifetime_as >= 202207L

using std::start_lifetime_as;
using std::start_lifetime_as_array;

#else

// NOLINTBEGIN

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp* start_lifetime_as(void* __p) noexcept {
#if __glibcxx_is_implicit_lifetime >= 202302L
  static_assert(std::is_implicit_lifetime_v<_Tp>);
#endif
  auto __q = reinterpret_cast<_Tp*>(__p);
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__q) : "0"(__q), "m"(*__q));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp const* start_lifetime_as(void const* __p) noexcept {
#if __glibcxx_is_implicit_lifetime >= 202302L
  static_assert(std::is_implicit_lifetime_v<_Tp>);
#endif
  auto __q = reinterpret_cast<const _Tp*>(__p);
  auto __r = reinterpret_cast<_Tp*>(const_cast<void*>(__p));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__r) : "0"(__q), "m"(*__q));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp volatile* start_lifetime_as(void volatile* __p) noexcept {
#if __glibcxx_is_implicit_lifetime >= 202302L
  static_assert(std::is_implicit_lifetime_v<_Tp>);
#endif
  auto __q = reinterpret_cast<volatile _Tp*>(__p);
  auto __r = reinterpret_cast<_Tp*>(const_cast<void*>(__p));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__r) : "0"(__q), "m"(*__q));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp const volatile* start_lifetime_as(void const volatile* __p) noexcept {
#if __glibcxx_is_implicit_lifetime >= 202302L
  static_assert(std::is_implicit_lifetime_v<_Tp>);
#endif
  auto __q = reinterpret_cast<const volatile _Tp*>(__p);
  auto __r = reinterpret_cast<_Tp*>(const_cast<void*>(__p));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__r) : "0"(__q), "m"(*__q));
  return __q;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvla-extension"
#endif

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp* start_lifetime_as_array(void* __p, size_t __n) noexcept {
  auto __q = reinterpret_cast<_Tp*>(__p);
  if (!__n)
    return __q;
  auto __r = (__extension__ reinterpret_cast<_Tp(*)[__n]>(__p));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__r) : "0"(__q), "m"(*__r));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp const* start_lifetime_as_array(void const* __p, size_t __n) noexcept {
  auto __q = reinterpret_cast<_Tp const*>(__p);
  if (!__n)
    return __q;
  auto __r = (__extension__ reinterpret_cast<_Tp const(*)[__n]>(__p));
  auto __s = (__extension__ reinterpret_cast<_Tp(*)[__n]>(const_cast<void*>(__p)));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__s) : "0"(__q), "m"(*__r));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp volatile* start_lifetime_as_array(void volatile* __p, size_t __n) noexcept {
  auto __q = reinterpret_cast<_Tp volatile*>(__p);
  if (!__n)
    return __q;
  auto __r = (__extension__ reinterpret_cast<_Tp volatile(*)[__n]>(__p));
  auto __s = (__extension__ reinterpret_cast<_Tp(*)[__n]>(const_cast<void*>(__p)));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__s) : "0"(__q), "m"(*__r));
  return __q;
}

template<typename _Tp>
[[__gnu__::__always_inline__]]
inline _Tp const volatile* start_lifetime_as_array(void const volatile* __p, size_t __n) noexcept {
  auto __q = reinterpret_cast<_Tp const volatile*>(__p);
  if (!__n)
    return __q;
  auto __r = (__extension__ reinterpret_cast<_Tp const volatile(*)[__n]>(__p));
  auto __s = (__extension__ reinterpret_cast<_Tp(*)[__n]>(const_cast<void*>(__p)));
  __asm__ __volatile__("" : "=g"(__q), "=m"(*__s) : "0"(__q), "m"(*__r));
  return __q;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif

// NOLINTEND

} // namespace mono
