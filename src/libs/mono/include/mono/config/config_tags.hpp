/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file config_tags.hpp
 * @date 26/05/2026
 * @brief Configuration tag types for compile-time config identification
 */

#pragma once

// --- Includes ---

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---
#include <type_traits>

// --- System ---


namespace mono::config {

struct BaseOptionTag { };

template<typename T>
concept OptionTag = std::is_base_of_v<BaseOptionTag, T>;

namespace tag {

struct Mandatory : BaseOptionTag { };

struct Default : BaseOptionTag { };

} // namespace tag

} // namespace mono::config
