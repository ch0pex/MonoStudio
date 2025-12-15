#pragma once

#include <boost/container/stable_vector.hpp>

namespace mono {

template<typename T>
using stable_vector = boost::container::stable_vector<T>;

}
