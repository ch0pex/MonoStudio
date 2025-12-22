#pragma once

#include <type_traits>
namespace mono::config {

struct BaseOptionTag { };

template<typename T>
concept OptionTag = std::is_base_of_v<BaseOptionTag, T>;

namespace tag {

struct Mandatory : BaseOptionTag { };

struct Default : BaseOptionTag { };

} // namespace tag

} // namespace mono::config
