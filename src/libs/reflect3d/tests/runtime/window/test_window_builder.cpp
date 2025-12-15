#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <GLFW/glfw3.h>
#include <doctest/doctest.h>

#include <reflect3d/window/window_builder.hpp>
#include <reflect3d/window/window_config.hpp>
#include <reflect3d/window/window_handle.hpp>

TEST_SUITE("WindowBuilder") {

  TEST_CASE("Create siemple window") {
    REQUIRE(glfwInit() == GLFW_TRUE);

    rf3d::WindowBuilder builder(
        rf3d::config::Window {
          .title      = "TestWindow",
          .resolution = {.width = 800, .height = 600},
        }
    );

    auto window = builder.build();
    REQUIRE(window.native_handle() != nullptr);

    int width  = 0;
    int height = 0;
    glfwGetWindowSize(window.native_handle(), &width, &height);
    CHECK(width == 800);
    CHECK(height == 600);

    char const* title = glfwGetWindowTitle(window.native_handle());

    CHECK(std::string(title) == "TestWindow");

    glfwDestroyWindow(window.native_handle());
  }

  TEST_CASE("Asignar callbacks y comprobar encadenamiento") {
    static bool focus_called = false;
    auto focus_callback      = [](GLFWwindow* /*w*/, int /*f*/) { focus_called = true; };

    rf3d::WindowBuilder builder;
    builder.on_focus(focus_callback).on_close([](GLFWwindow*) { }).on_size([](GLFWwindow*, int, int) { });

    auto window = builder.build();
    REQUIRE(window.native_handle() != nullptr);

    // Llamamos manualmente al callback para simular un evento
    auto cb = glfwSetWindowFocusCallback(window.native_handle(),
                                         nullptr); // obtener callback anterior
    REQUIRE(cb != nullptr);
    cb(window.native_handle(), 1);
    CHECK(focus_called == true);

    glfwDestroyWindow(window.native_handle());
  }

  TEST_CASE("Registrar default callbacks") {
    REQUIRE(glfwInit() == GLFW_TRUE);

    rf3d::WindowBuilder builder;
    builder.default_callbacks();
    auto window = builder.build();

    REQUIRE(window.native_handle() != nullptr);

    // Solo verificamos que no crash al llamar a un callback default
    auto cb = glfwSetWindowCloseCallback(window.native_handle(), nullptr);

    REQUIRE(cb != nullptr);

    cb(window.native_handle());

    glfwDestroyWindow(window.native_handle());
  }

} // TEST_SUITE
