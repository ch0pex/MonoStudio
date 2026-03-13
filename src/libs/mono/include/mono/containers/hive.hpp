#pragma once

#include <map>
#include <plf_colony.h>
#include <unordered_map>

namespace mono {

template<class T, class Alloc = std::allocator<T>, plf::priority priority = plf::performance>
using hive = plf::colony<T, Alloc, priority>;

} // namespace mono
