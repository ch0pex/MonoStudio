
#include <mono/execution/signals.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("setup_signals devuelve un stop_token activo") {
  auto token = mono::ex::setup_signals();
  REQUIRE_FALSE(token.stop_requested());
}

TEST_CASE("setup_signals solicita stop al recibir SIGINT") {

  auto token = mono::ex::setup_signals();

  // Emulate SIGINT
  raise(SIGINT);

  // Esperar un momento para que el handler se ejecute
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  REQUIRE(token.stop_requested());
}

TEST_CASE("setup_signals solicita stop al recibir SIGTERM") {
  auto token = mono::ex::setup_signals();

  // Emulate SIGTERM
  raise(SIGTERM);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  REQUIRE(token.stop_requested());
}
