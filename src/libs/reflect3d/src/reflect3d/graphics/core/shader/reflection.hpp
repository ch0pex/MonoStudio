/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file reflection.hpp
 * @date 07/05/2026
 * @brief Shader reflection utilities
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/detail/slang_format_map.hpp"
#include "reflect3d/graphics/core/pso_descriptor.hpp"
#include "reflect3d/graphics/core/shader/program.hpp"
#include "reflect3d/graphics/core/shader/types.hpp"

// --- External dependencies ---
#include <slang.h>

// --- STD ---
#include <cstdint>
#include <string_view>
#include <vector>

// --- System ---

namespace rf3d::shader {

namespace detail {

inline bool is_varying_input(slang::VariableLayoutReflection* param) {

  if (param == nullptr) {
    throw std::invalid_argument("Parameter reflection cannot be null");
  }

  for (std::uint32_t c = 0; c < param->getCategoryCount(); ++c) {
    if (param->getCategoryByIndex(c) == slang::VaryingInput) {
      return true;
    }
  }
  return false;
}


inline VertexBufferBinding extract_vertex_buffer_binding(slang::TypeLayoutReflection* layout) {
  std::vector<VertexBindingAttribute> attributes;
  std::uint32_t cumulative_offset = 0;

  for (std::uint32_t j = 0; j < layout->getFieldCount(); ++j) {
    auto* field_layout = layout->getFieldByIndex(j);

    char const* semantic = field_layout->getSemanticName();
    if (semantic != nullptr and std::string_view(semantic).starts_with("SV_")) {
      continue;
    }

    Format const fmt = map_slang_format_to_rf3d(field_layout->getType());
    if (fmt == Format::unknown) {
      continue;
    }

    attributes.emplace_back(cumulative_offset, fmt);
    cumulative_offset += format_byte_size(fmt);
  }

  return {
    .byte_stride = cumulative_offset,
    .attributes  = std::move(attributes),
  };
}


} // namespace detail


/**
 * @brief Finds vertex buffer bindings from shader reflection data.
 *
 * This function looks for varying input
 * parameters in the vertex shader entry point(s) and extracts their field information to construct vertex buffer
 * bindings. It ignores fields with semantics starting with "SV_" (system values) and only considers fields with
 * recognized formats.
 *
 * @param program Shader program
 * @return Vertex buffer bindings extracted from the shader program reflection.
 */
inline std::vector<VertexBufferBinding> find_vertex_bindings(Program const& program) {
  std::vector<VertexBufferBinding> bindings;

  for (auto* entry_point: program.entry_points()) {
    if (entry_point->getStage() != SLANG_STAGE_VERTEX) {
      continue;
    }

    for (std::uint32_t i = 0; i < entry_point->getParameterCount(); ++i) {
      auto* param = entry_point->getParameterByIndex(i);

      if (not detail::is_varying_input(param))
        continue;

      auto* type        = param->getType();
      auto* type_layout = param->getTypeLayout();

      if (type->getKind() != slang::TypeReflection::Kind::Struct) {
        continue;
      }

      auto binding = detail::extract_vertex_buffer_binding(type_layout);
      if (not binding.attributes.empty()) {
        bindings.emplace_back(std::move(binding));
      }
    }
  }

  return bindings;
}

namespace detail {

// inline void (slang::TypeLayoutReflection* type_layout, shader::Stage stage, ) { }
// class SlangPipelineReflector {
// public:
//     PipelineLayoutData reflectPipelineLayout(slang::ProgramLayout* programLayout) {
//         PipelineLayoutData pipelineData;
//
//         // Reserve space for the global scope descriptor set (Set 0 usually)
//         pipelineData.descriptorSets.push_back({0, {}});
//
//         // 1. Process Global Parameters
//         processElementLayout(
//             programLayout->getGlobalParamsTypeLayout(),
//             ShaderStage::All,
//             pipelineData,
//             0 // Targeting the first descriptor set
//         );
//
//         // 2. Process Entry Point Parameters
//         int entryPointCount = programLayout->getEntryPointCount();
//         for (int i = 0; i < entryPointCount; ++i) {
//             auto entryPoint = programLayout->getEntryPointByIndex(i);
//             ShaderStage stage = mapSlangStage(entryPoint->getStage());
//
//             processElementLayout(
//                 entryPoint->getTypeLayout(),
//                 stage,
//                 pipelineData,
//                 0 // Appended to the global set or managed based on your engine's logic
//             );
//         }
//
//         return pipelineData;
//     }
//
// private:
//     void processElementLayout(
//         slang::TypeLayoutReflection* typeLayout,
//         ShaderStage stageFlags,
//         PipelineLayoutData& pipelineData,
//         int currentSetIndex)
//     {
//         // 1. Check for automatically introduced uniform buffer (Scalar variables)
//         if (typeLayout->getSize() > 0) {
//             BindingGroupLayout uboBinding{};
//             uboBinding.index =
//             static_cast<std::uint32_t>(pipelineData.descriptorSets[currentSetIndex].bindings.size());
//             uboBinding.count = 1;
//             uboBinding.type = BindingType::UniformBuffer;
//             uboBinding.stage = stageFlags;
//
//             pipelineData.descriptorSets[currentSetIndex].bindings.push_back(uboBinding);
//         }
//
//         // 2. Extract explicit descriptor ranges (Textures, Samplers, etc.)
//         int relativeSetIndex = 0;
//         int rangeCount = typeLayout->getDescriptorSetDescriptorRangeCount(relativeSetIndex);
//
//         for (int i = 0; i < rangeCount; ++i) {
//             slang::BindingType slangBindingType = typeLayout->getDescriptorSetDescriptorRangeType(relativeSetIndex,
//             i);
//
//             // Push constants belong to the pipeline, not the descriptor set
//             if (slangBindingType == slang::BindingType::PushConstant) {
//                 continue;
//             }
//
//             BindingGroupLayout resourceBinding{};
//             resourceBinding.index =
//             static_cast<std::uint32_t>(pipelineData.descriptorSets[currentSetIndex].bindings.size());
//             resourceBinding.count =
//             static_cast<std::uint32_t>(typeLayout->getDescriptorSetDescriptorRangeDescriptorCount(relativeSetIndex,
//             i)); resourceBinding.type = mapSlangBinding(slangBindingType); resourceBinding.stage = stageFlags;
//
//             pipelineData.descriptorSets[currentSetIndex].bindings.push_back(resourceBinding);
//         }
//
//         // 3. Process Sub-Objects (Nested ParameterBlocks or Push Constants)
//         int subObjectCount = typeLayout->getSubObjectRangeCount();
//         for (int i = 0; i < subObjectCount; ++i) {
//             auto bindingRangeIndex = typeLayout->getSubObjectRangeBindingRangeIndex(i);
//             slang::BindingType subObjType = typeLayout->getBindingRangeType(bindingRangeIndex);
//             auto leafLayout = typeLayout->getBindingRangeLeafTypeLayout(bindingRangeIndex);
//
//             if (subObjType == slang::BindingType::PushConstant) {
//                 PushConstantRangeData pcRange{};
//                 pcRange.offset = 0;
//                 pcRange.size = static_cast<std::uint32_t>(leafLayout->getElementTypeLayout()->getSize());
//                 pcRange.stage = stageFlags;
//
//                 if (pcRange.size > 0) {
//                     pipelineData.pushConstants.push_back(pcRange);
//                 }
//             }
//             else if (subObjType == slang::BindingType::ParameterBlock) {
//                 // Nested parameter blocks create a NEW descriptor set
//                 std::uint32_t newSetIndex = static_cast<std::uint32_t>(pipelineData.descriptorSets.size());
//                 pipelineData.descriptorSets.push_back({newSetIndex, {}});
//
//                 processElementLayout(leafLayout, stageFlags, pipelineData, newSetIndex);
//             }
//         }
//     }
//
//     BindingType mapSlangBinding(slang::BindingType slangType) {
//         switch (slangType) {
//             case slang::BindingType::Sampler: return BindingType::Sampler;
//             case slang::BindingType::Texture: return BindingType::Texture;
//             case slang::BindingType::ConstantBuffer: return BindingType::UniformBuffer;
//             // Add other mappings as needed (Storage buffers, etc.)
//             default: return BindingType::Unknown;
//         }
//     }
//
//     ShaderStage mapSlangStage(SlangStage stage) {
//         switch (stage) {
//             case SLANG_STAGE_VERTEX: return ShaderStage::Vertex;
//             case SLANG_STAGE_FRAGMENT: return ShaderStage::Fragment;
//             case SLANG_STAGE_COMPUTE: return ShaderStage::Compute;
//             default: return ShaderStage::All;
//         }
//     }
// };

// inline void add_global_scope_layout(slang::TypeLayoutReflection* type_layout, PipelineLayout& layout) {
//
//   if() {
//
//   }
//
// }
//
// inline void add_entrypoints_layout(slang::TypeLayoutReflection* type_layout, PipelineLayout& layout) {
// pro
//
// }
} // namespace detail

// 1. Convert from Slang native enum to Engine agnostic enum
inline shader::Stage from_native(SlangStage stage) {
  switch (stage) {
    case SLANG_STAGE_VERTEX:
      return shader::Stage::vertex;
    case SLANG_STAGE_FRAGMENT:
      return shader::Stage::fragment; // Maps to SLANG_STAGE_PIXEL as well
    case SLANG_STAGE_COMPUTE:
      return shader::Stage::compute;
    case SLANG_STAGE_HULL:
      return shader::Stage::tessellation_control;
    case SLANG_STAGE_DOMAIN:
      return shader::Stage::tessellation_evaluation;
    case SLANG_STAGE_GEOMETRY:
      return shader::Stage::geometry;
    case SLANG_STAGE_NONE:
      return shader::Stage::none;
    default:
      return shader::Stage::all;
  }
}

// 2. Convert from Engine agnostic enum to Slang native enum
inline SlangStage to_native(shader::Stage stage) {
  auto bits     = static_cast<std::uint8_t>(stage);
  auto all_bits = static_cast<std::uint8_t>(shader::Stage::all);

  if ((bits & all_bits) == all_bits) {
    return SLANG_STAGE_NONE;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::vertex)) != 0) {
    return SLANG_STAGE_VERTEX;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::fragment)) != 0) {
    return SLANG_STAGE_FRAGMENT;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::compute)) != 0) {
    return SLANG_STAGE_COMPUTE;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::tessellation_control)) != 0) {
    return SLANG_STAGE_HULL;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::tessellation_evaluation)) != 0) {
    return SLANG_STAGE_DOMAIN;
  }
  if ((bits & static_cast<std::uint8_t>(shader::Stage::geometry)) != 0) {
    return SLANG_STAGE_GEOMETRY;
  }
  return SLANG_STAGE_NONE;
}

inline BindingType from_native(slang::BindingType slangType) {
  switch (slangType) {
    case slang::BindingType::ConstantBuffer:
      return BindingType::constant_buffer;

    case slang::BindingType::Sampler:
      return BindingType::sampler_state;

    case slang::BindingType::Texture:
      return BindingType::texture_2D;

    case slang::BindingType::MutableTexture:
      return BindingType::rw_texture_2D;

    // Map both typed (Buffer<T>) and raw (ByteAddressBuffer) to standard storage buffer
    case slang::BindingType::TypedBuffer:
    case slang::BindingType::RawBuffer:
      return BindingType::storage_buffer;

    // Map mutable versions (RWBuffer<T>, RWByteAddressBuffer) to RW storage buffer
    case slang::BindingType::MutableTypedBuffer:
    case slang::BindingType::MutableRawBuffer:
      return BindingType::rw_storage_buffer;

    default:
      return BindingType::unknown;
  }
}

/**
 * @brief Processes a Slang ParameterBlock and appends its bindings to the pipeline layout.
 *
 * Each Slang ParameterBlock maps to a DescriptorSet (Vulkan), DescriptorTable (DX12),
 * Argument Buffer (Metal), or Binding Group (WebGPU).
 *
 * Inside every parameter block there are multiple things to consider:
 * - A Uniform Buffer will be implicitly inserted in the first binding if any ordinary data is present.
 * - Then the rest of the bindings are added.
 * - ParameterBlocks might be nested, so to sort them properly iteration uses depth-first pre-ordering.
 *
 * @param element_layout The type layout of the element inside the ParameterBlock.
 * @param stage The shader stage this block belongs to.
 * @param layout The pipeline layout to append the new descriptor set and bindings to.
 */
inline void
process_parameter_block(slang::TypeLayoutReflection* element_layout, shader::Stage stage, PipelineLayout& layout) {
  // We need to introduce and reserve some space for the current set
  std::size_t const set_index = layout.sets.size();
  layout.sets.emplace_back();

  // if the Thing in ParameterBlock<Thing> has any ordinary data (e.g. float, double, vec, etc) element_layout.size
  // will be bigger than 0 and this means slang introduced an uniform buffer.
  if (element_layout->getSize() > 0) {
    layout.sets[set_index].bindings.emplace_back(1, BindingType::constant_buffer, stage);
  }

  // add_ranges
  std::int64_t relative_set_index = 0;
  std::int64_t const range_count  = element_layout->getDescriptorSetDescriptorRangeCount(relative_set_index);
  for (std::int64_t range_index = 0; range_index < range_count; ++range_index) {
    auto binding_type = element_layout->getDescriptorSetDescriptorRangeType(relative_set_index, range_index);

    // Push Constant are ignored in this step they wil be proccesed as a subrange
    if (binding_type == slang::BindingType::PushConstant) {
      continue;
    }

    layout.sets[set_index].bindings.emplace_back( //
      element_layout->getDescriptorSetDescriptorRangeDescriptorCount(relative_set_index, range_index),
      from_native(binding_type),
      stage
    );
  }

  // add_subranges
  for (int i = 0; i < element_layout->getSubObjectRangeCount(); ++i) {
    auto binding_range_idx = element_layout->getSubObjectRangeBindingRangeIndex(i);
    auto sub_obj_type      = element_layout->getBindingRangeType(binding_range_idx);
    auto* leaf_layout      = element_layout->getBindingRangeLeafTypeLayout(binding_range_idx);

    if (sub_obj_type == slang::BindingType::PushConstant) {
      auto range_size = leaf_layout->getElementTypeLayout()->getSize();

      if (range_size > 0) {
        layout.push_constants.emplace_back(0, range_size, stage);
      }
    }
    else if (sub_obj_type == slang::BindingType::ParameterBlock) {
      process_parameter_block(leaf_layout, stage, layout);
    }
  }
}

inline PipelineLayout find_pipeline_layout(Program const& program) {
  PipelineLayout layout;

  auto* shader_layout = program.layout();

  // First we process global parameter block
  process_parameter_block(shader_layout->getGlobalParamsTypeLayout(), shader::Stage::all, layout);

  // Then we need to process entry point specific parameters
  for (auto* entry_point: program.entry_points()) {
    auto entry_point_stage = from_native(entry_point->getStage());
    process_parameter_block(entry_point->getTypeLayout(), entry_point_stage, layout);
  }

  // With the current algorithm some sets might be empty and those need to be removed
  // An empty set (ParameterBlock) would be a set that is exclusively composed by other sets (ParameterBlock)
  auto [first, last] = std::ranges::remove_if(layout.sets, [](SetBindingGroup const& set) { //
    return set.bindings.empty();
  });
  layout.sets.erase(first, last);

  return layout;
}

} // namespace rf3d::shader
