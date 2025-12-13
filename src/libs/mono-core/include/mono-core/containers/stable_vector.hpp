#pragma once

#include <boost/container/stable_vector.hpp>

namespace rflect {

template<typename T>
using stable_vector = boost::container::stable_vector<T>;

}
