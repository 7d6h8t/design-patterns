#pragma once
#include <cstdint>
#include <windef.h>

namespace capture_utils {
struct RectF {
  float left;
  float top;
  float right;
  float bottom;
};

struct PointF {
  float x;
  float y;
};
}  // namespace capture_utils

class RectUtils {
 public:
  static float GetWidth(const capture_utils::RectF& rect);
  static int32_t GetWidth(const RECT& rect);

  static float GetHeight(const capture_utils::RectF& rect);
  static int32_t GetHeight(const RECT& rect);

  static capture_utils::PointF GetCenterPoint(const capture_utils::RectF& rect);
  static POINT GetCenterPoint(const RECT& rect);

  // center point based resize
  static void ResizeRect(capture_utils::RectF& rect, const float width,
                         const float height);
  static void ResizeRect(RECT& rect, const int32_t width, const int32_t height);

  // left top point based move
  static void MoveRect(capture_utils::RectF& rect,
                       const capture_utils::PointF& left_top);
  static void MoveRect(RECT& rect, const POINT& left_top);
};
