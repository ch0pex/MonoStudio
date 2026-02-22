#pragma once

#include "reflect3d/graphics/hri.hpp"
#include "reflect3d/graphics/vk/vk_traits.hpp"

namespace rf3d::gfx {

// ----------------------------------
// ------- Supported backends -------
// ----------------------------------

using Vulkan = vk::Traits;
using Dx12   = void;
using Metal  = void;
using Gl     = void;

#if defined(RF3D_USE_VULKAN)
using default_backend = Vulkan;
#elif defined(RF3D_USE_DX12)
using default_backend = Dx12;
#elif defined(RF3D_USE_METAL)
using default_backend = Metal;
#elif defined(RF3D_USE_GL)
using default_backend = Gl;
#endif

static_assert(Hri<Vulkan>, "Vulkan backend does not satisfy HRI concept");

} // namespace rf3d::gfx
