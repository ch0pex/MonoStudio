#pragma once

#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/api/fence.hpp"
#include "reflect3d/graphics/api/gpu.hpp"
#include "reflect3d/graphics/api/pso.hpp"
#include "reflect3d/graphics/api/surface.hpp"

namespace rf3d::gfx::wip {

template<typename T>
concept RenderHardwareInterface = //
    Gpu<typename T::Gpu> and //
    Surface<typename T::Surface> and //
    Buffer<typename T::Buffer> and //
    Fence<typename T::Fence> and //
    CommandList<typename T::CommandList> and //
    PipelineState<typename T::PipelineState>; //


} // namespace rf3d::gfx::wip
