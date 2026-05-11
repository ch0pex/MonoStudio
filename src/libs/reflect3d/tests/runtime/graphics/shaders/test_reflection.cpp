/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_reflection.cpp
 * @date 07/05/2026
 * @brief Test shader reflection
 *
 */

// --- Includes ---

// --- Dependencies ---

#include <reflect3d/graphics/core/shader/compiler.hpp>
#include <slang.h>

// --- External dependencies ---

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// --- STD ---

#include <filesystem>

// --- System ---

TEST_SUITE_BEGIN("Shader");

TEST_CASE("Reflection exposes all global parameters with correct names") {
  using namespace rf3d::shader;

  std::filesystem::path const shader_file =
      std::filesystem::path {mono::assets_path} / "shaders/test_bindless_shader.slang";
  auto const program = compile<SpirV>(shader_file).value();

  CHECK_FALSE(program.bytecode().empty());

  auto parameters = program.parameters();
  CHECK(std::ranges::distance(parameters) == 6);
  for (auto [idx, parameter]: parameters | std::views::enumerate) {
    LOG_INFO("Parameter {}: name='{}', type='{}'", idx, parameter->getName(), parameter->getType()->getName());
    CHECK(std::string {parameter->getName()} == std::format("gParameter{}", idx));
  }
}

TEST_CASE("Reflection exposes all entry points with correct names and stages") {
  using namespace rf3d::shader;

  std::filesystem::path const shader_file =
      std::filesystem::path {mono::assets_path} / "shaders/test_bindless_shader.slang";
  auto const program = compile<SpirV>(shader_file).value();

  auto entry_points = program.entry_points();
  CHECK(std::ranges::distance(entry_points) == 2);
  for (auto [idx, entry_point]: entry_points | std::views::enumerate) {
    LOG_INFO("Entry point {}: name='{}'", idx, entry_point->getName());
    CHECK(
        std::string {entry_point->getName()} ==
        std::format("{}Main", entry_point->getStage() == SLANG_STAGE_VERTEX ? "vert" : "frag")
    );
  }
}

TEST_SUITE_END();
