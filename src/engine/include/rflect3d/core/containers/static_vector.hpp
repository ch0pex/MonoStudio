#pragma once

#include <boost/container/static_vector.hpp>

namespace rflect {

template<typename T, std::size_t N>
using static_vector = boost::container::static_vector<T, N>;

}
