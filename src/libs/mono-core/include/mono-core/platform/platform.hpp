#pragma once

#include <string_view>

namespace rflect::os {

#ifdef WIN32

inline std::string_view constexpr name = "windows";

#else

inline std::string_view constexpr name = "linux";

#endif

} // namespace rflect::os
