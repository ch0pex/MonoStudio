#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

#include <mono/logging/logger.hpp>

#include <string>

namespace rf3d::gfx::vk {

constexpr std::string error_message(core::Result const result) noexcept { // clang-format off
  switch (result) {
    case core::Result::eSuccess:                                         return "Command successfully completed";
    case core::Result::eNotReady:                                        return "A fence or query has not yet completed";
    case core::Result::eTimeout:                                         return "A wait operation has not completed in the specified time";
    case core::Result::eEventSet:                                        return "An event is signaled";
    case core::Result::eEventReset:                                      return "An event is unsignaled";
    case core::Result::eIncomplete:                                      return "A return array was too small for the result";
    case core::Result::eErrorOutOfHostMemory:                            return "A host memory allocation has failed";
    case core::Result::eErrorOutOfDeviceMemory:                          return "A device memory allocation has failed";
    case core::Result::eErrorInitializationFailed:                       return "Initialization of an object could not be completed for implementation-specific reasons";
    case core::Result::eErrorDeviceLost:                                 return "The logical or physical device has been lost";
    case core::Result::eErrorMemoryMapFailed:                            return "Mapping of a memory object has failed";
    case core::Result::eErrorLayerNotPresent:                            return "A requested layer is not present or could not be loaded";
    case core::Result::eErrorExtensionNotPresent:                        return "A requested extension is not supported";
    case core::Result::eErrorFeatureNotPresent:                          return "A requested feature is not supported";
    case core::Result::eErrorIncompatibleDriver:                         return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
    case core::Result::eErrorTooManyObjects:                             return "Too many objects of the type have already been created";
    case core::Result::eErrorFormatNotSupported:                         return "A requested format is not supported on this device";
    case core::Result::eErrorFragmentedPool:                             return "A pool allocation has failed due to fragmentation of the pool's memory";
    case core::Result::eErrorUnknown:                                    return "An unknown error has occurred";
    case core::Result::eErrorOutOfPoolMemory:                            return "A pool memory allocation has failed";
    case core::Result::eErrorInvalidExternalHandle:                      return "An external handle is not a valid handle of the specified type";
    case core::Result::eErrorFragmentation:                              return "A descriptor pool creation has failed due to fragmentation";
    case core::Result::ePipelineCompileRequired:                         return "A requested pipeline creation would have required compilation, but the application requested compilation to not be performed";
    case core::Result::eErrorSurfaceLostKHR:                             return "A surface is no longer available";
    case core::Result::eErrorNativeWindowInUseKHR:                       return "The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again";
    case core::Result::eSuboptimalKHR:                                   return "A swapchain no longer matches the surface properties exactly, but can still be used";
    case core::Result::eErrorOutOfDateKHR:                               return "A surface has changed in such a way that it is no longer compatible with the swapchain";
    case core::Result::eErrorIncompatibleDisplayKHR:                     return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image";
    case core::Result::eErrorValidationFailedEXT:                        return "A validation layer found an error";
    case core::Result::eErrorInvalidShaderNV:                            return "One or more shaders failed to compile or link";
    case core::Result::eErrorNotPermittedKHR:                            return "The operation is not permitted";
    case core::Result::eThreadIdleKHR:                                   return "A deferred operation is not complete but there is currently no work for this thread to do at the time of this call";
    case core::Result::eThreadDoneKHR:                                   return "A deferred operation is not complete but there is no work remaining to assign to additional threads";
    case core::Result::eOperationDeferredKHR:                            return "A deferred operation was requested and at least some of the work was deferred";
    case core::Result::eOperationNotDeferredKHR:                         return "A deferred operation was requested and no operations were deferred";
    case core::Result::eErrorInvalidOpaqueCaptureAddress:                return "A buffer creation or memory allocation failed because the requested address is not available";
    // case core::Result::eErrorFullScreenExclusiveModeLostEXT:             return "An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exclusive full-screen access";
    case core::Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT:     return "The requested DRM format modifier plane layout is invalid";
    default:                                                           return "Unknown VkResult";
  }
} // clang-format on

} // namespace rf3d::gfx::vk
