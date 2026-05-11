/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file program.hpp
 * @date 09/05/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/core/pso_states.hpp"
#include "reflect3d/graphics/core/shader/types.hpp"

// --- Dependencies ---

// --- External dependencies ---
#include <slang-com-ptr.h>

// --- STD ---
#include <ranges>
#include <stdexcept>
#include <vector>

// --- System ---


namespace rf3d::shader {

struct ProgramException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};


class Program {
public:
  using code_type    = Bytecode;
  using program_type = Slang::ComPtr<slang::IComponentType>;
  using layout_type  = Layout;

  Program(program_type&& program, code_type code) : code(code), program(std::move(program)) {
    if (code.empty()) {
      throw std::runtime_error("Failed to compile shader program.");
    }
    if (this->program == nullptr) {
      throw std::runtime_error("Failed to compile shader program.");
    }
  }

  [[nodiscard]] code_type const& bytecode() const { return code; }

  [[nodiscard]] auto parameters() const {
    auto& layout = *program->getLayout();

    auto get_parameter = [&layout](std::uint32_t const index) {
      return layout.getParameterByIndex(index); //
    };

    return std::views::iota(0U, layout.getParameterCount()) | std::views::transform(get_parameter);
  }

  [[nodiscard]] auto entry_points() const {
    auto& layout = *program->getLayout();

    auto get_entry_point = [&layout](std::uint32_t const index) {
      return layout.getEntryPointByIndex(index); //
    };

    return std::views::iota(0U, layout.getEntryPointCount()) | std::views::transform(get_entry_point);
  }

private:
  code_type code {};
  program_type program;
};


} // namespace rf3d::shader
