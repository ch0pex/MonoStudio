#pragma once

#include "reflect3d/graphics/rhi_concepts.hpp"
#include "reflect3d/graphics/vk/vk_renderer.hpp"

namespace rf3d::gfx {

using Vulkan = vk::Renderer;
using Dx12   = void;
using Metal  = void;
using OpenGL = void;

#if defined(RF3D_VULKAN_DEFAULT)
using DefaultBackend = Vulkan;
#elif defined(RF3D_DX12_DEFAULT)
using DefaultBackend = Dx12;
#elif defined(RF3D_METAL_DEFAULT)
using DefaultBackend = Metal;
#elif defined(RF3D_GL_DEFAULT)
using DefaultBackend = OpenGL;
#endif

static_assert(RenderHardwareInterface<Vulkan>, "Vulkan backend does not satisfy HRI concept");

} // namespace rf3d::gfx
