#pragma once

#ifdef MONO_USE_CPP_MODULES
import std;
#else
#include <string_view>
#endif

namespace mono::os {

#ifdef WIN32

inline std::string_view constexpr name = "windows";

#else

inline std::string_view constexpr name = "linux";

#endif

} // namespace mono::os
