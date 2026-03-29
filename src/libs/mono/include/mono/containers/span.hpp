#pragma once

#include <span>

namespace mono {

#if __cpp_lib_span_initializer_list >= 202311L

template<typename T, std::size_t Extent = std::dynamic_extent>
using span = std::span<T, Extent>;

#else

template<typename T, std::size_t Extent = std::dynamic_extent>
struct span : std::span<T, Extent> {
  using std::span<T, Extent>::span;

  constexpr explicit(Extent != std::dynamic_extent) span(std::initializer_list<std::remove_cv_t<T>> il) noexcept
    requires std::is_const_v<T>
    : span(il.begin(), il.size()) { }
};

template<typename Type, size_t ArrayExtent>
span(Type (&)[ArrayExtent]) -> span<Type, ArrayExtent>; // NOLINT(cppcoreguidelines-avoid-c-arrays)

template<typename Type, size_t ArrayExtent>
span(std::array<Type, ArrayExtent>&) -> span<Type, ArrayExtent>;

template<typename Type, size_t ArrayExtent>
span(std::array<Type, ArrayExtent> const&) -> span<Type const, ArrayExtent>;

template<std::contiguous_iterator Iter, typename End>
span(Iter, End) -> span<std::remove_reference_t<std::iter_reference_t<Iter>>>;

template<std::ranges::contiguous_range Range>
span(Range&&) -> span<std::remove_reference_t<std::ranges::range_reference_t<Range&>>>;

#endif

} // namespace mono
