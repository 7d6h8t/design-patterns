#include "pch.h"
#include "zoom_viewport.h"

#include "coordinate_system.h"
#include "resource_provider.h"
#include "rect_utils.h"

ZoomViewport::ZoomViewport(CoordinateSystem& coordinate,
                           ResourceProvider& resource)
    : coordinate_(coordinate), resource_(resource) {}

void ZoomViewport::Init(const SIZE& physical_size, const SIZE& screen_size) {
  zoom_ratio_ = zoom_default_percent_;
  coordinate_.Init(physical_size, screen_size);
  Update();
  UpdateScroll();
}

void ZoomViewport::Zoom(bool zoom_in, const POINT& screen_point) {
  if (SetZoomRatio(zoom_in) == false) return;

  capture_utils::RectF zoom_logical = coordinate_.CalculateLogicalRect(
      coordinate_.GetPhysicalRect(), coordinate_.GetWindowScreenRect());

  {
    // zoom rect
    RectUtils::ResizeRect(
        zoom_logical,
        static_cast<float>(RectUtils::GetWidth(zoom_logical) * zoom_ratio_ /
                           zoom_default_percent_),
        static_cast<float>(RectUtils::GetHeight(zoom_logical) * zoom_ratio_ /
                           zoom_default_percent_));
  }

  {
    // move rect
    auto physical_point = coordinate_.WindowScreenToPhysical(screen_point);
    capture_utils::PointF zoom_logical_point =
        coordinate_.PhysicalToBasedLogical(physical_point, zoom_logical);
    capture_utils::PointF logical_point =
        coordinate_.PhysicalToLogical(physical_point);

    auto offset_x = zoom_logical_point.x - logical_point.x;
    auto offset_y = zoom_logical_point.y - logical_point.y;
    RectUtils::MoveRect(zoom_logical, {zoom_logical.left - offset_x,
                                       zoom_logical.top - offset_y});
  }

  {
    // viewing 영역보다 zoom rect 가 작을 경우 center로 move
    auto logical_screen = coordinate_.GetLogicalScreenRect();
    auto left =
        RectUtils::GetWidth(zoom_logical) < RectUtils::GetWidth(logical_screen)
            ? RectUtils::GetCenterPoint(logical_screen).x -
                  RectUtils::GetWidth(zoom_logical) / 2
            : zoom_logical.left;

    auto top = RectUtils::GetHeight(zoom_logical) <
                       RectUtils::GetHeight(logical_screen)
                   ? RectUtils::GetCenterPoint(logical_screen).y -
                         RectUtils::GetHeight(zoom_logical) / 2
                   : zoom_logical.top;

    RectUtils::MoveRect(zoom_logical, {left, top});
  }

  coordinate_.UpdateLogicalRect(zoom_logical);

  Update();
  UpdateScroll();
}

void ZoomViewport::Move(const POINT& last, const POINT& current) {
  auto last_logical_point = coordinate_.WindowScreenToLogical(last);
  auto current_logical_point = coordinate_.WindowScreenToLogical(current);

  auto offset_x = current_logical_point.x - last_logical_point.x;
  auto offset_y = current_logical_point.y - last_logical_point.y;

  auto logical = coordinate_.GetLogicalRect();
  RectUtils::MoveRect(logical,
                      {logical.left + offset_x, logical.top + offset_y});
  coordinate_.UpdateLogicalRect(logical);

  Update();
  UpdateScroll();
}

ZoomViewport::ZoomRatio ZoomViewport::GetZoomRatio() { return zoom_ratio_; }

bool ZoomViewport::SetZoomRatio(bool zoom_in) noexcept {
  uint32_t zoom_ratio = zoom_ratio_;
  if (zoom_in)
    zoom_ratio += zoom_step_percent_;
  else
    zoom_ratio -= zoom_step_percent_;

  if (zoom_ratio < zoom_min_percent_ || zoom_ratio > zoom_max_percent_)
    return false;

  zoom_ratio_ = zoom_ratio;
  return true;
}

void ZoomViewport::UpdateScroll() {
  {
    // update horizontal scroll bar
    SCROLLINFO si{};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;
    GetScrollInfo(resource_.GetRenderingWindow(), SB_HORZ, &si);
    si.nMin = static_cast<int32_t>(coordinate_.GetLogicalRect().left);
    si.nMax = static_cast<int32_t>(coordinate_.GetLogicalRect().right);
    si.nPage = static_cast<uint32_t>(
        RectUtils::GetWidth(coordinate_.GetLogicalScreenRect()));
    SetScrollInfo(resource_.GetRenderingWindow(), SB_HORZ, &si, true);
  }

  {
    // update vertical scroll bar
    SCROLLINFO si{};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;
    GetScrollInfo(resource_.GetRenderingWindow(), SB_VERT, &si);
    si.nMin = static_cast<int32_t>(coordinate_.GetLogicalRect().top);
    si.nMax = static_cast<int32_t>(coordinate_.GetLogicalRect().bottom);
    si.nPage = static_cast<uint32_t>(
        RectUtils::GetHeight(coordinate_.GetLogicalScreenRect()));
    SetScrollInfo(resource_.GetRenderingWindow(), SB_VERT, &si, true);
  }

  auto logical_screen = coordinate_.GetLogicalScreenRect();
  SetScrollPos(resource_.GetRenderingWindow(), SB_HORZ,
               static_cast<int32_t>(logical_screen.left), true);
  SetScrollPos(resource_.GetRenderingWindow(), SB_VERT,
               static_cast<int32_t>(logical_screen.top), true);
}

void ZoomViewport::Update() {
  auto logical = coordinate_.GetLogicalRect();
  viewport_ =
      CD3D11_VIEWPORT(logical.left, logical.top, RectUtils::GetWidth(logical),
                      RectUtils::GetHeight(logical));
}
