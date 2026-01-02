
#include <mono/meta/concepts.hpp>

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace {

// InPack tests
static_assert(mono::meta::IsInPack<int, float, double, int, char>);
static_assert(not mono::meta::IsInPack<std::string, float, double, int, char>);
static_assert(mono::meta::IsInPack<std::optional<int>, std::vector<int>, std::optional<int>, std::unique_ptr<int>>);

// IsTemplateParameterOf tests
static_assert(mono::meta::IsTemplateParameterOf<int, std::vector<int>>);
static_assert(mono::meta::IsTemplateParameterOf<std::string, std::optional<std::string>>);
static_assert(mono::meta::IsTemplateParameterOf<double, std::unique_ptr<double>>);
static_assert(not mono::meta::IsTemplateParameterOf<float, std::vector<int>>);
static_assert(not mono::meta::IsTemplateParameterOf<char, std::optional<int>>);
static_assert(not mono::meta::IsTemplateParameterOf<void, std::unique_ptr<int>>);
static_assert(mono::meta::IsTemplateParameterOf<int, std::tuple<int, double, std::string>>);
static_assert(mono::meta::IsTemplateParameterOf<int, std::variant<int, double, std::string>>);

} // namespace
