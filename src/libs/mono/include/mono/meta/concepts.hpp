#pragma once

#include "mono/meta/detail/specialization_of.hpp"

#include <concepts>

namespace mono::meta {

template<typename T>
concept MovableValue = std::move_constructible<std::decay_t<T>> and std::constructible_from<std::decay_t<T>, T>;

template<typename Type, template<typename...> class Template>
concept SpecializationOf = detail::SpecializationOf<Type, Template>::value;


} // namespace mono::meta
