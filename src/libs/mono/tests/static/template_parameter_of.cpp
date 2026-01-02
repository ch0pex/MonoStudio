
#include <mono/meta/concepts.hpp>

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace {

// InPack tests
static_assert(mono::meta::in_pack<int, float, double, int, char>);
static_assert(not mono::meta::in_pack<std::string, float, double, int, char>);
static_assert(mono::meta::in_pack<std::optional<int>, std::vector<int>, std::optional<int>, std::unique_ptr<int>>);

// IsTemplateParameterOf tests
static_assert(mono::meta::template_parameter_of<int, std::vector<int>>);
static_assert(mono::meta::template_parameter_of<std::string, std::optional<std::string>>);
static_assert(mono::meta::template_parameter_of<double, std::unique_ptr<double>>);
static_assert(not mono::meta::template_parameter_of<float, std::vector<int>>);
static_assert(not mono::meta::template_parameter_of<char, std::optional<int>>);
static_assert(not mono::meta::template_parameter_of<void, std::unique_ptr<int>>);
static_assert(mono::meta::template_parameter_of<int, std::tuple<int, double, std::string>>);
static_assert(mono::meta::template_parameter_of<int, std::variant<int, double, std::string>>);

} // namespace
