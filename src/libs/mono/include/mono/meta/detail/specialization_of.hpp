/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file specialization_of.hpp
 * @date 26/05/2026
 * @brief Specialization-of concept for template type introspection
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <type_traits>

// --- System ---


namespace mono::meta::detail {

template<class Type, template<typename...> class Template>
struct SpecializationOf : std::false_type { };

template<template<typename...> class Template, typename... Args>
struct SpecializationOf<Template<Args...>, Template> : std::true_type { };

} // namespace mono::meta::detail
