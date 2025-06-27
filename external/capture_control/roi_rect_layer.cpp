#include "pch.h"
#include "roi_rect_layer.h"

#include "device_resources.h"
#include "include/com_utils.h"
#include "dx_struct.h"
#include "compute_shader_helper.h"
#include "coordinate_provider.h"
#include "resource_provider.h"
#include "rect_utils.h"

using Microsoft::WRL::ComPtr;

ROIRectLayer::ROIRectLayer(CoordinateProvider& coordinate,
                           ResourceProvider& resource)
    : DrawLayer(coordinate, resource) {}

void ROIRectLayer::Enable(bool enable) { enable_ = enable; }

bool ROIRectLayer::IsEnable() const { return enable_; }

void ROIRectLayer::SetRoiScreenRect(const RECT& rect) {
  const auto physical_rect = coordinate_.ScreenToPhysical(rect);
  SetRoiPhysicalRect(physical_rect);
}

void ROIRectLayer::SetRoiPhysicalRect(const RECT& rect) {
  RECT roi = rect;
  if (rect.left > rect.right) {
    roi.left = rect.right;
    roi.right = rect.left;
  }

  if (rect.top > rect.bottom) {
    roi.top = rect.bottom;
    roi.bottom = rect.top;
  }

  auto frame = resource_.GetFrameSize();
  roi.left = roi.left <= 0 ? 0 : roi.left;
  roi.right = roi.right >= frame.cx ? frame.cx : roi.right;
  roi.top = roi.top <= 0 ? 0 : roi.top;
  roi.bottom = roi.bottom >= frame.cy ? frame.cy : roi.bottom;

  roi_rect_ = roi;
}

void ROIRectLayer::Draw(const RECT& rect) {
  if (IsEnable() == false) return;

  auto immediate_context =
      dx::DeviceResources::GetSingleton().GetDeviceContext();

  // activate blend
  float blend_factor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  uint32_t sample_mask = 0xffffffff;
  immediate_context->OMSetBlendState(blend_state_.Get(), blend_factor,
                                     sample_mask);

  // Pixel Shader (PS)
  immediate_context->PSSetShader(pixel_shader_.Get(), nullptr, 0);

  dx::FrameBuffer frame_buffer_data = {
      {static_cast<uint32_t>(resource_.GetFrameSize().cx),
       static_cast<uint32_t>(resource_.GetFrameSize().cy)}};
  auto frame_buffer =
      dx::ComputeShaderHelper::CreateConstBuffer(frame_buffer_data);

  immediate_context->PSSetConstantBuffers(0, 1, frame_buffer.GetAddressOf());
  immediate_context->PSSetConstantBuffers(1, 1, roi_buffer_.GetAddressOf());

  capture_utils::PointF uv_lt = ConvertToUV({rect.left, rect.top});
  capture_utils::PointF uv_rb = ConvertToUV({rect.right, rect.bottom});

  dx::RoiRectBufferF uv_roi{
      {static_cast<float>(uv_lt.x), static_cast<float>(uv_lt.y)},
      {static_cast<float>(uv_rb.x), static_cast<float>(uv_rb.y)}};

  D3D11_MAPPED_SUBRESOURCE mapped_resource{};
  immediate_context->Map(roi_buffer_.Get(), 0,
                         D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

  *(reinterpret_cast<DirectX::XMFLOAT2*>(mapped_resource.pData)) =
      uv_roi.left_top_;
  *(reinterpret_cast<DirectX::XMFLOAT2*>(mapped_resource.pData) + 1) =
      uv_roi.right_bottom_;
  immediate_context->Unmap(roi_buffer_.Get(), 0);

  immediate_context->DrawIndexed(6, 0, 0);

  // deactivate blend
  immediate_context->OMSetBlendState(nullptr, nullptr, sample_mask);
}

void ROIRectLayer::Draw() {
  if (RectUtils::GetWidth(roi_rect_) < 1 || RectUtils::GetHeight(roi_rect_) < 1)
    return;

  Draw(roi_rect_);
}

RECT ROIRectLayer::GetRoiPhysicalRect() { return roi_rect_; }

void ROIRectLayer::CreatePixelShader() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  // Load and create shaders.
  auto ps_blob =
      dx::ComputeShaderHelper::LoadShaderBlob(roi_rect_pixel_shader_path_);

  com::ThrowIfFailed(
      device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr,
                                pixel_shader_.ReleaseAndGetAddressOf()));
}

void ROIRectLayer::CreateConstBuffer() {
  dx::RoiRectBufferF init_roi_buffer_data = {{0.0, 0.0}, {0.0, 0.0}};
  roi_buffer_ = dx::ComputeShaderHelper::CreateConstBuffer(init_roi_buffer_data,
                                                           D3D11_USAGE_DYNAMIC);
}
