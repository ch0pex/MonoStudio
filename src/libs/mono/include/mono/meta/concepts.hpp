#pragma once

#include "mono/meta/detail/specialization_of.hpp"
#include "mono/meta/detail/template_parameter_of.hpp"

#include <concepts>
#include <type_traits>

namespace mono::meta {

template<typename T>
concept movable_value = std::move_constructible<std::decay_t<T>> and std::constructible_from<std::decay_t<T>, T>;

template<typename Type, template<typename...> class Template>
concept specialization_of = detail::SpecializationOf<Type, Template>::value;

template<typename T, typename... Ts>
concept in_pack = detail::ContainsType<T, Ts...>;

template<typename Type, typename Container>
concept template_parameter_of = detail::TemplateParameterOf<Type, Container>::value;

} // namespace mono::meta
