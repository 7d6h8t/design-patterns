#include "pch.h"
#include "coordinate_system.h"

#include <cmath>

#include <utils.h>

void CoordinateSystem::Init(const SIZE& physical_size,
                            const SIZE& screen_size) {
  window_screen_ = {0, 0, screen_size.cx, screen_size.cy};
  physical_ = {0, 0, physical_size.cx, physical_size.cy};
  logical_screen_ = {0, 0, static_cast<float>(physical_size.cx),
                     static_cast<float>(physical_size.cy)};
  logical_ = CalculateLogicalRect(physical_, window_screen_);
}

capture_utils::RectF CoordinateSystem::CalculateLogicalRect(
    const RECT& physical_rect, const RECT& window_screen_rect) const {
  RECT physical = physical_rect;
  RECT screen = window_screen_rect;

  if (IsRectEmpty(&physical))
    throw std::exception("Calculate logical rect error");

  float physical_ratio = static_cast<float>(RectUtils::GetWidth(physical)) /
                         static_cast<float>(RectUtils::GetHeight(physical));
  float screen_ratio = static_cast<float>(RectUtils::GetWidth(screen)) /
                       static_cast<float>(RectUtils::GetHeight(screen));

  capture_utils::RectF logical = {
      0, 0, static_cast<float>(RectUtils::GetWidth(physical)),
      static_cast<float>(RectUtils::GetHeight(physical))};
  auto width = RectUtils::GetWidth(logical);
  auto height = RectUtils::GetHeight(logical);
  if (screen_ratio > physical_ratio) {
    // screen w / h ratio > physical w / h ratio의 경우 -> w 감소
    width = RectUtils::GetWidth(physical) / screen_ratio * physical_ratio;
  } else {
    // screen h / w ratio < physical h / w ratio의 경우 - > h 감소
    physical_ratio = static_cast<float>(RectUtils::GetHeight(physical)) /
                     static_cast<float>(RectUtils::GetWidth(physical));
    screen_ratio = static_cast<float>(RectUtils::GetHeight(screen)) /
                   static_cast<float>(RectUtils::GetWidth(screen));

    height = RectUtils::GetHeight(physical) / screen_ratio * physical_ratio;
  }
  RectUtils::ResizeRect(logical, width, height);

  return logical;
}

capture_utils::PointF CoordinateSystem::WindowScreenToLogical(
    const POINT& window_screen) const {
  float scale_x = static_cast<float>(RectUtils::GetWidth(physical_)) /
                  RectUtils::GetWidth(window_screen_);
  float scale_y = static_cast<float>(RectUtils::GetHeight(physical_)) /
                  RectUtils::GetHeight(window_screen_);

  auto logical_x = window_screen.x * scale_x;
  auto logical_y = window_screen.y * scale_y;

  return {logical_x, logical_y};
}

capture_utils::RectF CoordinateSystem::WindowScreenToLogical(
    const RECT& window_screen) const {
  capture_utils::PointF left_top =
      WindowScreenToLogical(POINT{window_screen.left, window_screen.top});
  capture_utils::PointF right_bottom =
      WindowScreenToLogical(POINT{window_screen.right, window_screen.bottom});

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

POINT CoordinateSystem::WindowScreenToPhysical(
    const POINT& window_screen) const {
  capture_utils::PointF logical = WindowScreenToLogical(window_screen);
  POINT physical = LogicalToPhysical(logical);

  return physical;
}

RECT CoordinateSystem::WindowScreenToPhysical(const RECT& window_screen) const {
  capture_utils::RectF logical = WindowScreenToLogical(window_screen);
  RECT physical = LogicalToPhysical(logical);

  return physical;
}

POINT CoordinateSystem::LogicalToPhysical(
    const capture_utils::PointF& logical) const {
  POINT physical{};
  physical.x = std::lround(
      (logical.x - static_cast<int32_t>(logical_.left)) *
      (RectUtils::GetWidth(physical_) / RectUtils::GetWidth(logical_)));
  physical.y = std::lround(
      (logical.y - static_cast<int32_t>(logical_.top)) *
      (RectUtils::GetHeight(physical_) / RectUtils::GetHeight(logical_)));

  return physical;
}

RECT CoordinateSystem::LogicalToPhysical(const capture_utils::RectF& logical) const {
  POINT left_top =
      LogicalToPhysical(capture_utils::PointF{logical.left, logical.top});
  POINT right_bottom =
      LogicalToPhysical(capture_utils::PointF{logical.right, logical.bottom});

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

RECT CoordinateSystem::LogicalToWindowScreen(const capture_utils::RectF& logical) const {
  POINT left_top =
      LogicalToWindowScreen(capture_utils::PointF{logical.left, logical.top});
  POINT right_bottom = LogicalToWindowScreen(
      capture_utils::PointF{logical.right, logical.bottom});

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

POINT CoordinateSystem::LogicalToWindowScreen(
    const capture_utils::PointF& logical) const {
  float scale_x = static_cast<float>(RectUtils::GetWidth(window_screen_)) /
                  RectUtils::GetWidth(physical_);
  float scale_y = static_cast<float>(RectUtils::GetHeight(window_screen_)) /
                  RectUtils::GetHeight(physical_);

  auto window_screen_x = static_cast<LONG>(logical.x * scale_x);
  auto window_screen_y = static_cast<LONG>(logical.y * scale_y);

  return {window_screen_x, window_screen_y};
}

capture_utils::PointF CoordinateSystem::PhysicalToLogical(const POINT& physical) const {
  return PhysicalToBasedLogical(physical, logical_);
}

capture_utils::RectF CoordinateSystem::PhysicalToLogical(const RECT& physical) const {
  capture_utils::PointF left_top =
      PhysicalToBasedLogical(POINT{physical.left, physical.top}, logical_);
  capture_utils::PointF right_bottom =
      PhysicalToBasedLogical(POINT{physical.right, physical.bottom}, logical_);

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

capture_utils::PointF CoordinateSystem::PhysicalToBasedLogical(
    const POINT& physical, const capture_utils::RectF& based_logical) const {
  capture_utils::PointF logical{};
  logical.x = (physical.x * RectUtils::GetWidth(based_logical)) /
                  RectUtils::GetWidth(physical_) +
              based_logical.left;
  logical.y = (physical.y * RectUtils::GetHeight(based_logical)) /
                  RectUtils::GetHeight(physical_) +
              based_logical.top;

  return logical;
}

capture_utils::RectF CoordinateSystem::PhysicalToBasedLogical(
    const RECT& physical, const capture_utils::RectF& based_logical) const {
  capture_utils::PointF left_top =
      PhysicalToBasedLogical(POINT{physical.left, physical.top}, based_logical);
  capture_utils::PointF right_bottom = PhysicalToBasedLogical(
      POINT{physical.right, physical.bottom}, based_logical);

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

POINT CoordinateSystem::PhysicalToWindowScreen(const POINT& physical) const {
  auto logical = PhysicalToLogical(physical);
  auto window_screen = LogicalToWindowScreen(logical);

  return window_screen;
}

RECT CoordinateSystem::PhysicalToWindowScreen(const RECT& physical) const {
  POINT left_top = PhysicalToWindowScreen(POINT{physical.left, physical.top});
  POINT right_bottom =
      PhysicalToWindowScreen(POINT{physical.right, physical.bottom});

  return {left_top.x, left_top.y, right_bottom.x, right_bottom.y};
}

RECT CoordinateSystem::GetWindowScreenRect() const { return window_screen_; }

RECT CoordinateSystem::GetPhysicalRect() const { return physical_; }

capture_utils::RectF CoordinateSystem::GetLogicalRect() const { return logical_; }

capture_utils::RectF CoordinateSystem::GetLogicalScreenRect() const { return logical_screen_; }

void CoordinateSystem::UpdateLogicalRect(const capture_utils::RectF& logical) {
  logical_ = logical;
}
