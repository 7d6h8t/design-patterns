#include "pch.h"
#include "center_point_layer.h"

#include "include/com_utils.h"
#include "device_resources.h"
#include "resource_provider.h"
#include "compute_shader_helper.h"
#include "dx_struct.h"

CenterPointLayer::CenterPointLayer(CoordinateProvider& coordinate,
                                   ResourceProvider& resource)
    : DrawLayer(coordinate, resource) {}

void CenterPointLayer::Enable(bool enable) { enable_ = enable; }

bool CenterPointLayer::IsEnable() const { return enable_; }

void CenterPointLayer::Draw() {
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

  immediate_context->DrawIndexed(6, 0, 0);

  // deactivate blend
  immediate_context->OMSetBlendState(nullptr, nullptr, sample_mask);
}

void CenterPointLayer::CreatePixelShader() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  // Load and create shaders.
  auto ps_blob =
      dx::ComputeShaderHelper::LoadShaderBlob(center_point_pixel_shader_path_);

  com::ThrowIfFailed(
      device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr,
                                pixel_shader_.ReleaseAndGetAddressOf()));
}

void CenterPointLayer::CreateConstBuffer() {}
