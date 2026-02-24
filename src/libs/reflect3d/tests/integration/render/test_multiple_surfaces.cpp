#include "common.hpp"
#include "reflect3d/graphics/rhi.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>


int main() { test::renderer<rf3d::impl::vk>(3, {}); }
