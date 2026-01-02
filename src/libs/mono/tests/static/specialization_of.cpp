
#include <mono/meta/concepts.hpp>

#include <expected>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

namespace {

// Test with vector
static_assert(mono::meta::specialization_of<std::vector<int>, std::vector>);
static_assert(mono::meta::specialization_of<std::vector<double>, std::vector>);
static_assert(mono::meta::specialization_of<std::vector<std::string>, std::vector>);

// Test with optional
static_assert(mono::meta::specialization_of<std::optional<int>, std::optional>);
static_assert(mono::meta::specialization_of<std::optional<void*>, std::optional>);

// Test with unique_ptr
static_assert(mono::meta::specialization_of<std::unique_ptr<int>, std::unique_ptr>);
static_assert(mono::meta::specialization_of<std::unique_ptr<double>, std::unique_ptr>);

// Negative tests - non-specializations
static_assert(not mono::meta::specialization_of<int, std::vector>);
static_assert(not mono::meta::specialization_of<std::string, std::vector>);
static_assert(not mono::meta::specialization_of<int, std::optional>);

// Expected example
static_assert(mono::meta::specialization_of<std::expected<int, std::runtime_error>, std::expected>);
static_assert(mono::meta::specialization_of<std::expected<std::string, int>, std::expected>);
static_assert(not mono::meta::specialization_of<std::expected<char, std::string>, std::optional>);
static_assert(not mono::meta::specialization_of<std::expected<float, double>, std::vector>);

// Std array
// static_assert(mono::meta::SpecializationOf<std::array<int, 10>, std::array>); do not work with nttps


} // namespace
