#include <reflect3d/graphics/core/pso_states.hpp>
#include <reflect3d/graphics/core/viewport.hpp>
#include <reflect3d/graphics/vk/detail/utils/vk_to_native.hpp>

using namespace rf3d;
using namespace rf3d::vk::detail;

namespace {

static_assert(to_native(PrimitiveTopology::point_list) == core::PrimitiveTopology::ePointList);
static_assert(to_native(PrimitiveTopology::line_list) == core::PrimitiveTopology::eLineList);
static_assert(to_native(PrimitiveTopology::line_strip) == core::PrimitiveTopology::eLineStrip);
static_assert(to_native(PrimitiveTopology::triangle_list) == core::PrimitiveTopology::eTriangleList);
static_assert(to_native(PrimitiveTopology::triangle_strip) == core::PrimitiveTopology::eTriangleStrip);
static_assert(to_native(PrimitiveTopology::triangle_fan) == core::PrimitiveTopology::eTriangleFan);

static_assert(to_native(ComparisonFunction::never) == core::CompareOp::eNever);
static_assert(to_native(ComparisonFunction::less) == core::CompareOp::eLess);
static_assert(to_native(ComparisonFunction::equal) == core::CompareOp::eEqual);
static_assert(to_native(ComparisonFunction::less_equal) == core::CompareOp::eLessOrEqual);
static_assert(to_native(ComparisonFunction::greater) == core::CompareOp::eGreater);
static_assert(to_native(ComparisonFunction::not_equal) == core::CompareOp::eNotEqual);
static_assert(to_native(ComparisonFunction::greater_equal) == core::CompareOp::eGreaterOrEqual);
static_assert(to_native(ComparisonFunction::always) == core::CompareOp::eAlways);

static_assert(to_native(FrontFace::counter_clockwise) == core::FrontFace::eCounterClockwise);
static_assert(to_native(FrontFace::clockwise) == core::FrontFace::eClockwise);

static_assert(to_native(CullMode::none) == core::CullModeFlagBits::eNone);
static_assert(to_native(CullMode::front) == core::CullModeFlagBits::eFront);
static_assert(to_native(CullMode::back) == core::CullModeFlagBits::eBack);

static_assert(to_native(PolygonMode::fill) == core::PolygonMode::eFill);
static_assert(to_native(PolygonMode::line) == core::PolygonMode::eLine);
static_assert(to_native(PolygonMode::point) == core::PolygonMode::ePoint);

static_assert(to_native(FillMode::solid) == core::PolygonMode::eFill);
static_assert(to_native(FillMode::wireframe) == core::PolygonMode::eLine);
static_assert(to_native(FillMode::point) == core::PolygonMode::ePoint);

static_assert(to_native(PsoType::graphics) == PsoType::graphics);
static_assert(to_native(PsoType::compute) == PsoType::compute);
static_assert(to_native(PsoType::mesh_shader) == PsoType::mesh_shader);

constexpr Viewport viewport {
  .rect =
      Rect2D {
        .offset = {1.0F, 2.0F},
        .extent = {3.0F, 4.0F},
      },
  .min_depth = 0.0F,
  .max_depth = 1.0F,
};

constexpr Rect2D scissor {
  .offset = {5.0F, 6.0F},
  .extent = {7.0F, 8.0F},
};

static_assert([] {
  constexpr auto vk_viewport = to_native(viewport);
  return vk_viewport.x == 1.0F && vk_viewport.y == 2.0F && vk_viewport.width == 3.0F && vk_viewport.height == 4.0F &&
         vk_viewport.minDepth == 0.0F && vk_viewport.maxDepth == 1.0F;
}());

static_assert([] {
  constexpr auto vk_scissor = to_native(scissor);
  return vk_scissor.offset.x == 5 && vk_scissor.offset.y == 6 && vk_scissor.extent.width == 7U &&
         vk_scissor.extent.height == 8U;
}());

} // namespace
