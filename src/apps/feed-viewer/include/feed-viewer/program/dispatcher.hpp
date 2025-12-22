#pragma once

#include "feed-viewer/program/config.hpp"

#include <print>

namespace feed {

inline void run_viewer(ProgramConfig const& config) { std::println("{}", config.feeds); }

} // namespace feed
