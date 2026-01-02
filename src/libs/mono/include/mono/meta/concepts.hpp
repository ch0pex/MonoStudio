#pragma once

#include "mono/meta/detail/specialization_of.hpp"
#include "mono/meta/detail/template_parameter_of.hpp"

#include <concepts>
#include <type_traits>

namespace mono::meta {

template<typename T>
concept MovableValue = std::move_constructible<std::decay_t<T>> and std::constructible_from<std::decay_t<T>, T>;

template<typename Type, template<typename...> class Template>
concept SpecializationOf = detail::SpecializationOf<Type, Template>::value;

template<typename T, typename... Ts>
concept IsInPack = detail::ContainsType<T, Ts...>;

template<typename Type, typename Container>
concept IsTemplateParameterOf = detail::TemplateParameterOf<Type, Container>::value;

} // namespace mono::meta
