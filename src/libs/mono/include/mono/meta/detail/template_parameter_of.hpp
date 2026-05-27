/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file template_parameter_of.hpp
 * @date 26/05/2026
 * @brief Template parameter extraction type traits
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <type_traits>

// --- System ---


namespace mono::meta::detail {

template<typename T, typename... Ts>
concept ContainsType = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename Type, typename Container>
struct TemplateParameterOf : std::false_type { };

template<typename Type, template<typename...> class Template, typename... Args>
struct TemplateParameterOf<Type, Template<Args...>> : std::bool_constant<ContainsType<Type, Args...>> { };

} // namespace mono::meta::detail
