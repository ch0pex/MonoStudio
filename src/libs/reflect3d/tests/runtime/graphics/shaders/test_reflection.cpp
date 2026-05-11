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

// --- External dependencies ---

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// --- STD ---

#include <filesystem>

// --- System ---

TEST_SUITE_BEGIN("Shader");

TEST_CASE("Test reflection") {

  using namespace rf3d::shader;

  std::filesystem::path shader_file = std::filesystem::path {mono::assets_path} / "shaders/test_bindless_shader.slang";
  auto const program                = compile<SpirV>(shader_file).value();

  CHECK_FALSE(program.bytecode().empty());

  auto parameters   = program.parameters();
  auto entry_points = program.entry_points();

  CHECK(std::ranges::distance(parameters) == 6);
  for (auto [idx, parameter]: parameters | std::views::enumerate) {
    LOG_INFO("Parameter {}: name='{}', type='{}'", idx, parameter->getName(), parameter->getType()->getName());
    CHECK(std::string {parameter->getName()} == std::format("gParameter{}", idx));
  }

  CHECK(std::ranges::distance(entry_points) == 2);
  // for (auto [idx, entry_point]: entry_points | std::views::enumerate) {
  // LOG_INFO("Entry Point {}: name='{}', stage={}", idx, entry_point->getName(), entry_point->getStage());
  // CHECK(
  //     std::string {entry_point->getName()} ==
  //     std::format("{}Main", entry_point->getStage() == slang::ShaderStage::Vertex ? "vertex" : "fragment")
  // );
  // }
}

TEST_SUITE_END();
