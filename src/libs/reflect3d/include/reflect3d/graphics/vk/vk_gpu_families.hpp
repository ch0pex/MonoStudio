#pragma once

//
#include "reflect3d/graphics/vk/vk_gpu_families_detail.hpp"

//
#include <optional>
#include <ranges>
#include <unordered_map>


namespace rf3d::gfx::vk {

/*
 * Reflect3d will only support for now gpus whose graphics and present operations are handled by the same queue family
 * This is indeed the most common case on modern PC hardware and greatly simplifies the implementation
 * Android support might be hurt by this decision, but will be considered later
 */
enum class QueueFamilyType : std::uint8_t {
  Main = 0,
  Transfer,

  count,
};

inline std::string to_string(QueueFamilyType const type) {
  switch (type) {
    case QueueFamilyType::Main:
      return "Graphics/Present/Compute";
    case QueueFamilyType::Transfer:
      return "Transfer";
    default:
      return "Unknown";
  }
}

class GpuQueueFamilies {
public:
  using family_type      = QueueFamilyType;
  using family_info_type = core::DeviceQueueCreateInfo;

  explicit GpuQueueFamilies(raii::PhysicalDevice const& physical_device) {
    families[family_type::Main] = detail::find_main_family(physical_device);
    // TODO find comput and transfer families
  }

  [[nodiscard]] auto device_creation_infos() const {
    return families | std::views::values | std::ranges::to<std::vector<family_info_type>>();
  }

  template<QueueFamilyType FamilyType>
    requires(FamilyType < QueueFamilyType::count and FamilyType >= static_cast<QueueFamilyType>(0))
  [[nodiscard]] family_info_type family_info() const {
    if (auto const it = families.find(FamilyType); it != families.end()) {
      return it->second;
    }
    throw std::runtime_error(
        "Requested queue family type not found or not supported by your GPU: " + to_string(FamilyType)
    );
  }

private:
  std::unordered_map<family_type, family_info_type> families;
};

} // namespace rf3d::gfx::vk
