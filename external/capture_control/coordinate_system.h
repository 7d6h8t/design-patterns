#pragma once
#include <cstdint>
#include "rect_utils.h"

class CoordinateSystem {
 public:
  void Init(const SIZE& physical_size, const SIZE& screen_size);
  capture_utils::RectF CalculateLogicalRect(
      const RECT& physical_rect, const RECT& window_screen_rect) const;

  capture_utils::PointF WindowScreenToLogical(const POINT& window_screen) const;
  capture_utils::RectF WindowScreenToLogical(const RECT& window_screen) const;

  POINT WindowScreenToPhysical(const POINT& window_screen) const;
  RECT WindowScreenToPhysical(const RECT& window_screen) const;

  POINT LogicalToPhysical(const capture_utils::PointF& logical) const;
  RECT LogicalToPhysical(const capture_utils::RectF& logical) const;

  POINT LogicalToWindowScreen(const capture_utils::PointF& logical) const;
  RECT LogicalToWindowScreen(const capture_utils::RectF& logical) const;

  capture_utils::PointF PhysicalToLogical(const POINT& physical) const;
  capture_utils::RectF PhysicalToLogical(const RECT& physical) const;

  capture_utils::PointF PhysicalToBasedLogical(
      const POINT& physical, const capture_utils::RectF& based_logical) const;
  capture_utils::RectF PhysicalToBasedLogical(
      const RECT& physical, const capture_utils::RectF& based_logical) const;

  POINT PhysicalToWindowScreen(const POINT& physical) const;
  RECT PhysicalToWindowScreen(const RECT& physical) const;

  RECT GetWindowScreenRect() const;
  RECT GetPhysicalRect() const;
  capture_utils::RectF GetLogicalRect() const;
  capture_utils::RectF GetLogicalScreenRect() const;

  void UpdateLogicalRect(const capture_utils::RectF& logical);

 protected:
  RECT window_screen_ = {};
  RECT physical_ = {};
  capture_utils::RectF logical_ = {};
  capture_utils::RectF logical_screen_ = {};
};
