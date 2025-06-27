#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "rect_utils.h"

class CoordinateProvider;
class ResourceProvider;

class DrawLayer {
 public:
  DrawLayer(CoordinateProvider& coordinate, ResourceProvider& resource);
  virtual ~DrawLayer() = default;

  void Init();

 protected:
  virtual void CreatePixelShader() = 0;
  void CreateBlendState();
  virtual void CreateConstBuffer() = 0;
  capture_utils::PointF ConvertToUV(const POINT& coords);

  CoordinateProvider& coordinate_;
  ResourceProvider& resource_;

  Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state_ = nullptr;
};
