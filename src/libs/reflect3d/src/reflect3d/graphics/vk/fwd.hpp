#pragma once

#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/command_list_type.hpp"
#include "reflect3d/graphics/core/texture_info.hpp"

namespace rf3d::vk {


template<BufferUsage Usage, MemoryProperty Mem>
class Buffer;

template<TextureUsage Usage, bool owning>
class Texture;

template<CommandListType Type>
class CommandList;

class Surface;

class PipelineState;

class Semaphore;

class Fence;

struct Gpu;

class Surface;

} // namespace rf3d::vk
