#pragma once

#include "mono/meta/concepts.hpp"
#include "reflect3d/graphics/vk/gpu/vk_gpu_families.hpp"
#include "reflect3d/graphics/vk/gpu/vk_logical_device.hpp"

#include <mono/misc/passkey.hpp>

//
#include <concepts>
#include <type_traits>

namespace rf3d::gfx::vk {

template<typename T>
concept Queue =
    std::is_base_of_v<raii::Queue, T> and std::constructible_from<T, LogicalDevice const&, GpuQueueFamilies const&>;

struct GraphicQueue : raii::Queue {
  GraphicQueue(LogicalDevice const& device, GpuQueueFamilies const& queue_families) :
    raii::Queue {device.create_graphics_queue(queue_families.family_info<QueueFamilyType::Main>())} { }
};

struct PresentQueue : raii::Queue {
  PresentQueue(LogicalDevice const& device, GpuQueueFamilies const& queue_families) :
    raii::Queue {device.create_present_queue(queue_families.family_info<QueueFamilyType::Main>())} { }
};

struct ComputeQueue : raii::Queue {
  ComputeQueue(LogicalDevice const& device, GpuQueueFamilies const& queue_families) : raii::Queue {nullptr} {
    throw std::runtime_error("ComputeQueue not yet implemented");
  }
};

struct TransferQueue : raii::Queue {
  TransferQueue(LogicalDevice const& device, GpuQueueFamilies const& queue_families) : raii::Queue {nullptr} {
    throw std::runtime_error("TransferQueue not yet implemented");
  }
};

template<Queue... Types>
class Queues {
public:
  using graphics_type = GraphicQueue;
  using present_type  = PresentQueue;
  using compute_type  = ComputeQueue;
  using transfer_type = TransferQueue;

  Queues(LogicalDevice const& device, GpuQueueFamilies const& queue_families) :
    queues {Types(device, queue_families)...} { }


  [[nodiscard]] present_type const& present() const noexcept
    requires(mono::meta::in_pack<present_type, Types...>)
  {
    return std::get<present_type>(queues);
  }

  [[nodiscard]] graphics_type const& graphics() const noexcept
    requires(mono::meta::in_pack<graphics_type, Types...>)
  {
    return std::get<graphics_type>(queues);
  }

  [[nodiscard]] compute_type const& compute() const noexcept
    requires(mono::meta::in_pack<compute_type, Types...>)
  {
    return std::get<compute_type>(queues);
  }

  [[nodiscard]] transfer_type const& transfer() const noexcept
    requires(mono::meta::in_pack<transfer_type, Types...>)
  {
    return std::get<transfer_type>(queues);
  }

  template<Queue Q>
  [[nodiscard]] Q const& get() const noexcept
    requires(mono::meta::in_pack<Q, Types...>)
  {
    return std::get<Q>(queues);
  }

private:
  std::tuple<Types...> queues;
};


} // namespace rf3d::gfx::vk
