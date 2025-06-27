#include "pch.h"
#include "rect_utils.h"

using namespace capture_utils;

float RectUtils::GetWidth(const RectF& rect) { return rect.right - rect.left; }

int32_t RectUtils::GetWidth(const RECT& rect) { return rect.right - rect.left; }

float RectUtils::GetHeight(const RectF& rect) { return rect.bottom - rect.top; }

int32_t RectUtils::GetHeight(const RECT& rect) {
  return rect.bottom - rect.top;
}

PointF RectUtils::GetCenterPoint(const RectF& rect) {
  return PointF{(rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2};
}

POINT RectUtils::GetCenterPoint(const RECT& rect) {
  return POINT{(rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2};
}

void RectUtils::ResizeRect(RectF& rect, const float width, const float height) {
  float center_x = (rect.left + rect.right) / 2;
  float center_y = (rect.top + rect.bottom) / 2;

  rect.left = center_x - width / 2;
  rect.right = center_x + width / 2;
  rect.top = center_y - height / 2;
  rect.bottom = center_y + height / 2;
}

void RectUtils::ResizeRect(RECT& rect, const int32_t width,
                           const int32_t height) {
  int32_t center_x = (rect.left + rect.right) / 2;
  int32_t center_y = (rect.top + rect.bottom) / 2;

  rect.left = center_x - width / 2;
  rect.right = center_x + width / 2;
  rect.top = center_y - height / 2;
  rect.bottom = center_y + height / 2;
}

void RectUtils::MoveRect(RectF& rect, const PointF& left_top) {
  float width = GetWidth(rect);
  float height = GetHeight(rect);

  rect.left = left_top.x;
  rect.top = left_top.y;

  rect.right = rect.left + width;
  rect.bottom = rect.top + height;
}

void RectUtils::MoveRect(RECT& rect, const POINT& left_top) {
  int32_t width = GetWidth(rect);
  int32_t height = GetHeight(rect);

  rect.left = left_top.x;
  rect.top = left_top.y;

  rect.right = rect.left + width;
  rect.bottom = rect.top + height;
}
