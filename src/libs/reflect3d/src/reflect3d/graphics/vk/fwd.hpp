/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file fwd.hpp
 * @date 26/05/2026
 * @brief Forward declarations for all Vulkan backend types
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/command_list_type.hpp"
#include "reflect3d/graphics/core/texture_info.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


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
