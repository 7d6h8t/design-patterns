#include "pch.h"
#include "draw_layer.h"

#include "include/com_utils.h"
#include "device_resources.h"
#include "resource_provider.h"

DrawLayer::DrawLayer(CoordinateProvider& coordinate, ResourceProvider& resource)
    : coordinate_(coordinate), resource_(resource) {}

void DrawLayer::Init() {
  CreatePixelShader();
  CreateBlendState();
  CreateConstBuffer();
}

void DrawLayer::CreateBlendState() {
  D3D11_BLEND_DESC blend_desc = {};
  blend_desc.AlphaToCoverageEnable = false;
  blend_desc.IndependentBlendEnable = false;
  blend_desc.RenderTarget[0].BlendEnable = true;
  blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blend_desc.RenderTarget[0].RenderTargetWriteMask =
      D3D11_COLOR_WRITE_ENABLE_ALL;

  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateBlendState(
          &blend_desc, blend_state_.ReleaseAndGetAddressOf()));
}

capture_utils::PointF DrawLayer::ConvertToUV(const POINT& coords) {
  return {static_cast<float>(coords.x) / resource_.GetFrameSize().cx,
          static_cast<float>(coords.y) / resource_.GetFrameSize().cy};
}
