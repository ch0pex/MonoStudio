#pragma once


#include "feed-viewer/feed-viewer.hpp"

namespace feed {

inline void run_viewer(Config const& config) { std::println("{}", config.feeds); }

} // namespace feed
