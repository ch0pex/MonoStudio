#pragma once

#include <type_traits>

namespace mono::meta::detail {

template<typename T, typename... Ts>
concept ContainsType = std::disjunction_v<std::is_same<T, Ts>...>;

template<typename Type, typename Container>
struct TemplateParameterOf : std::false_type { };

template<typename Type, template<typename...> class Template, typename... Args>
struct TemplateParameterOf<Type, Template<Args...>> : std::bool_constant<ContainsType<Type, Args...>> { };

} // namespace mono::meta::detail
