#pragma once
#include <d3d11.h>
#include <cstdint>

#include "rect_utils.h"

class CoordinateSystem;
class ResourceProvider;

class ZoomViewport final {
 public:
  using ZoomRatio = uint32_t;

  ZoomViewport(CoordinateSystem& coordinate, ResourceProvider& resource);
  ~ZoomViewport() = default;

  void Init(const SIZE& physical_size, const SIZE& screen_size);
  void Zoom(bool zoom_in, const POINT& screen_point);
  void Move(const POINT& last, const POINT& current);

  ZoomRatio GetZoomRatio();

  const D3D11_VIEWPORT* operator&() const { return &viewport_; }
  operator D3D11_VIEWPORT() const { return viewport_; }

 private:
  bool SetZoomRatio(bool zoom_in) noexcept;
  void UpdateScroll();
  void Update();

  CoordinateSystem& coordinate_;
  ResourceProvider& resource_;
  D3D11_VIEWPORT viewport_{};
  ZoomRatio zoom_ratio_ = zoom_default_percent_;

  inline const static ZoomRatio zoom_default_percent_ = 100;
  inline const static ZoomRatio zoom_max_percent_ = 1000;
  inline const static ZoomRatio zoom_min_percent_ = 10;
  inline const static ZoomRatio zoom_step_percent_ = zoom_max_percent_ / 50;
};
