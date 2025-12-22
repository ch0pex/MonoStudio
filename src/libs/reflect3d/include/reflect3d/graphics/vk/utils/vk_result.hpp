#pragma once

#include "reflect3d/graphics/vk/utils/vk_exception.hpp"

#include <mono/logging/logger.hpp>

#include <vulkan/vulkan_core.h>

#include <format>
#include <source_location>
#include <string>

namespace rf3d::hri::vk {

using Result = VkResult;

constexpr std::string error_message(Result const result) noexcept { // clang-format off
  switch (result) {
    case VK_SUCCESS:                              return "Command successfully completed";
    case VK_NOT_READY:                            return "A fence or query has not yet completed";
    case VK_TIMEOUT:                              return "A wait operation has not completed in the specified time";
    case VK_EVENT_SET:                            return "An event is signaled";
    case VK_EVENT_RESET:                          return "An event is unsignaled";
    case VK_INCOMPLETE:                           return "A return array was too small for the result";
    case VK_ERROR_OUT_OF_HOST_MEMORY:             return "A host memory allocation has failed";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:           return "A device memory allocation has failed";
    case VK_ERROR_INITIALIZATION_FAILED:          return "Initialization of an object could not be completed for implementation-specific reasons";
    case VK_ERROR_DEVICE_LOST:                    return "The logical or physical device has been lost";
    case VK_ERROR_MEMORY_MAP_FAILED:              return "Mapping of a memory object has failed";
    case VK_ERROR_LAYER_NOT_PRESENT:              return "A requested layer is not present or could not be loaded";
    case VK_ERROR_EXTENSION_NOT_PRESENT:          return "A requested extension is not supported";
    case VK_ERROR_FEATURE_NOT_PRESENT:            return "A requested feature is not supported";
    case VK_ERROR_INCOMPATIBLE_DRIVER:            return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
    case VK_ERROR_TOO_MANY_OBJECTS:               return "Too many objects of the type have already been created";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:           return "A requested format is not supported on this device";
    case VK_ERROR_FRAGMENTED_POOL:                return "A pool allocation has failed due to fragmentation of the pool's memory";
    case VK_ERROR_UNKNOWN:                        return "An unknown error has occurred";
    case VK_ERROR_OUT_OF_POOL_MEMORY:             return "A pool memory allocation has failed";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:        return "An external handle is not a valid handle of the specified type";
    case VK_ERROR_FRAGMENTATION:                  return "A descriptor pool creation has failed due to fragmentation";
    case VK_PIPELINE_COMPILE_REQUIRED:            return "A requested pipeline creation would have required compilation, but the application requested compilation to not be performed";
    case VK_ERROR_SURFACE_LOST_KHR:               return "A surface is no longer available";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:       return "The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again";
    case VK_SUBOPTIMAL_KHR:                       return "A swapchain no longer matches the surface properties exactly, but can still be used";
    case VK_ERROR_OUT_OF_DATE_KHR:                return "A surface has changed in such a way that it is no longer compatible with the swapchain";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:       return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image";
    case VK_ERROR_VALIDATION_FAILED_EXT:          return "A validation layer found an error";
    case VK_ERROR_INVALID_SHADER_NV:              return "One or more shaders failed to compile or link";
    case VK_ERROR_NOT_PERMITTED_KHR:              return "The operation is not permitted";
    case VK_THREAD_IDLE_KHR:                      return "A deferred operation is not complete but there is currently no work for this thread to do at the time of this call";
    case VK_THREAD_DONE_KHR:                      return "A deferred operation is not complete but there is no work remaining to assign to additional threads";
    case VK_OPERATION_DEFERRED_KHR:               return "A deferred operation was requested and at least some of the work was deferred";
    case VK_OPERATION_NOT_DEFERRED_KHR:           return "A deferred operation was requested and no operations were deferred";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "A buffer creation or memory allocation failed because the requested address is not available";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exclusive full-screen access";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "The requested DRM format modifier plane layout is invalid";
    default:                                    return "Unknown VkResult";
  }
} 

constexpr std::string to_string(Result const result) noexcept {
  switch (result) {
    case VK_SUCCESS: return "VK_SUCCESS";
    case VK_NOT_READY: return "VK_NOT_READY";
    case VK_TIMEOUT: return "VK_TIMEOUT";
    case VK_EVENT_SET: return "VK_EVENT_SET";
    case VK_EVENT_RESET: return "VK_EVENT_RESET";
    case VK_INCOMPLETE: return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
    case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_PIPELINE_COMPILE_REQUIRED: return "VK_PIPELINE_COMPILE_REQUIRED";
    case VK_ERROR_NOT_PERMITTED: return "VK_ERROR_NOT_PERMITTED";
    case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
    case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
    case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
    case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
    case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
    case VK_INCOMPATIBLE_SHADER_BINARY_EXT: return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
    case VK_PIPELINE_BINARY_MISSING_KHR: return "VK_PIPELINE_BINARY_MISSING_KHR";
    case VK_ERROR_NOT_ENOUGH_SPACE_KHR: return "VK_ERROR_NOT_ENOUGH_SPACE_KHR";
    case VK_RESULT_MAX_ENUM: return "VK_RESULT_MAX_ENUM";
    default: return "UNKNOWN_VK_RESULT";
  }
} // clang-format on

} // namespace rf3d::hri::vk
