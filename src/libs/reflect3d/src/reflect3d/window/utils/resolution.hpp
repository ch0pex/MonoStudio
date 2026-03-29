#pragma once

#include <compare>
#include <cstdint>

namespace rf3d {

struct Resolution {
  std::uint16_t width {1920};
  std::uint16_t height {1080};

  std::strong_ordering operator<=>(Resolution const& rhs) const = default;
};

// --- Default Resolutions ---
inline constexpr Resolution default_resolution {};
inline constexpr Resolution null_resolution {.width = 0, .height = 0};
inline constexpr Resolution hd_resolution {.width = 1280, .height = 720};
inline constexpr Resolution full_hd_resolution {.width = 1920, .height = 1080};
inline constexpr Resolution ultra_hd_resolution {.width = 3840, .height = 2160};
inline constexpr Resolution four_k_resolution {.width = 4096, .height = 2160};
inline constexpr Resolution eight_k_resolution {.width = 7680, .height = 4320};
inline constexpr Resolution wide_fhd_resolution {.width = 2560, .height = 1080};
inline constexpr Resolution wide_qhd_resolution {.width = 3440, .height = 1440};

} // namespace rf3d
