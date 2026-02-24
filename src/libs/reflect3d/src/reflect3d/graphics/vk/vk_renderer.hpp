#pragma once

#include "reflect3d/graphics/core/frame_info.hpp"
#include "reflect3d/graphics/core/mesh.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/containers/hive.hpp>
#include <mono/misc/as_span.hpp>

namespace rf3d::gfx::vk {

class Surface;

/*
 * Vulkan renderer interface
 *
 * @note this might be a namespace once static reflection is adopted,
 * but for now it is a class to allow for static member functions and type traits.
 */
struct Renderer {
  /*********************
   *    Type Traits    *
   ********************/
  using surface_handle = Surface*; //

  /**********************
   *    Constructors    *
   **********************/

  Renderer() = delete;

  Renderer(Renderer const&) = delete;

  Renderer(Renderer&&) = delete;

  Renderer& operator=(Renderer const&) = delete;

  Renderer& operator=(Renderer&&) = delete;

  ~Renderer() = delete;

  /**************************
   *    Member functions    *
   **************************/

  static surface_handle create_surface(Window&& window);

  static void destroy_surface(surface_handle surface);

  static void render_surface(surface_handle surface, FrameInfo const& frame_info);

  static void add_mesh(Mesh const& mesh);

  static void add_render_item();

  static void set_option();

  static void get_option();
};


} // namespace rf3d::gfx::vk
