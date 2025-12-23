#pragma once

#include <boost/unordered/unordered_flat_map.hpp>

namespace mono {

template<
    typename K,
    typename V, //
    typename Hash      = std::hash<K>, //
    typename Pred      = std::equal_to<K>, //
    typename Allocator = std::allocator<std::pair<K const, V>> //
    > //
using unordered_flat_map = boost::unordered_flat_map<K, V, Hash, Pred, Allocator>;

}
