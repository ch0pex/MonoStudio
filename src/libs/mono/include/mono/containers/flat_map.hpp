#pragma once

#include <boost/container/flat_map.hpp>

namespace mono {

template<
    typename K, //
    typename V, //
    typename Compare   = std::less<K>, //
    typename Allocator = std::allocator<std::pair<K const, V>> //
    >
using flat_map = boost::container::flat_map<K, V, Compare, Allocator>;

}
