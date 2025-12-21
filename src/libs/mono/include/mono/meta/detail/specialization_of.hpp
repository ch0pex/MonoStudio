#pragma once

#include <type_traits>

namespace mono::meta::detail {

template<class Type, template<typename...> class Template>
struct SpecializationOf : std::false_type { };

template<template<typename...> class Template, typename... Args>
struct SpecializationOf<Template<Args...>, Template> : std::true_type { };

} // namespace mono::meta::detail
