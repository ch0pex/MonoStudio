#pragma once

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <type_traits>
#endif


namespace mono::meta::detail {

template<typename T, typename... Ts>
concept ContainsType = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename Type, typename Container>
struct TemplateParameterOf : std::false_type { };

template<typename Type, template<typename...> class Template, typename... Args>
struct TemplateParameterOf<Type, Template<Args...>> : std::bool_constant<ContainsType<Type, Args...>> { };

} // namespace mono::meta::detail
