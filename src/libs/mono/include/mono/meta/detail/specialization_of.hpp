#pragma once

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <type_traits>
#endif

namespace mono::meta::detail {

template<class Type, template<typename...> class Template>
struct SpecializationOf : std::false_type { };

template<template<typename...> class Template, typename... Args>
struct SpecializationOf<Template<Args...>, Template> : std::true_type { };

} // namespace mono::meta::detail
