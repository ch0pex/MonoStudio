#pragma once

#include <plf_colony.h>

namespace mono {

template<class T, class Alloc = std::allocator<T>, plf::priority priority = plf::performance>
using hive = plf::colony<T, Alloc, priority>;

template<class T, class Alloc = std::allocator<T>>
class DeletionQueue { };


class QueueDeletedHive {
public:
private:
};

} // namespace mono
