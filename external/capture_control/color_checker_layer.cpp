#include "pch.h"
#include "color_checker_layer.h"

#include "device_resources.h"
#include "include/com_utils.h"
#include "dx_struct.h"
#include "compute_shader_helper.h"
#include "coordinate_provider.h"
#include "resource_provider.h"
#include "rect_utils.h"

ColorCheckerLayer::ColorCheckerLayer(CoordinateProvider& coordinate,
                                     ResourceProvider& resource)
    : DrawLayer(coordinate, resource) {}

void ColorCheckerLayer::Enable(bool enable) { enable_ = enable; }

bool ColorCheckerLayer::IsEnable() const { return enable_; }

std::vector<std::array<POINT, 4>> ColorCheckerLayer::GetPhysicalCheckerBoxs() {
  return checker_boxs_;
}

void ColorCheckerLayer::SetCheckerBoxs(const std::vector<POINT>& checker_boxs) {
  if (checker_boxs.size() % 4 != 0)
    throw std::exception("Checker board draw error");

  std::vector<std::array<POINT, 4>> boxs;
  boxs.reserve(checker_boxs.size() / 4);

  for (size_t i = 0; i < checker_boxs.size(); i += 4) {
    std::array<POINT, 4> box = {checker_boxs[i], checker_boxs[i + 1],
                                checker_boxs[i + 2], checker_boxs[i + 3]};
    boxs.push_back(box);
  }

  checker_boxs_ = boxs;
}

void ColorCheckerLayer::Draw(
    const std::vector<std::array<POINT, 4>>& checker_boxs) {
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
  immediate_context->PSSetConstantBuffers(1, 1,
                                          color_checker_buffer_.GetAddressOf());

  CheckerBoxs uv_boxs{};
  int32_t i = 0;
  for (const auto& checker_box : checker_boxs) {
    uv_boxs.check_box[i] = {
        ConvertToUV(checker_box[0]), ConvertToUV(checker_box[1]),
        ConvertToUV(checker_box[2]), ConvertToUV(checker_box[3])};
    ++i;
  }

  D3D11_MAPPED_SUBRESOURCE mapped_resource{};
  immediate_context->Map(color_checker_buffer_.Get(), 0,
                         D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);

  for (int32_t i = 0; i < 24; ++i) {
    *(reinterpret_cast<capture_utils::PointF*>(
          reinterpret_cast<Box*>(mapped_resource.pData) + i) +
      0) = uv_boxs.check_box[i].lt;
    *(reinterpret_cast<capture_utils::PointF*>(
          reinterpret_cast<Box*>(mapped_resource.pData) + i) +
      1) = uv_boxs.check_box[i].rt;
    *(reinterpret_cast<capture_utils::PointF*>(
          reinterpret_cast<Box*>(mapped_resource.pData) + i) +
      2) = uv_boxs.check_box[i].rb;
    *(reinterpret_cast<capture_utils::PointF*>(
          reinterpret_cast<Box*>(mapped_resource.pData) + i) +
      3) = uv_boxs.check_box[i].lb;
  }

  immediate_context->Unmap(color_checker_buffer_.Get(), 0);

  immediate_context->DrawIndexed(6, 0, 0);

  // deactivate blend
  immediate_context->OMSetBlendState(nullptr, nullptr, sample_mask);
}

void ColorCheckerLayer::Draw() { Draw(checker_boxs_); }

void ColorCheckerLayer::CreatePixelShader() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  // Load and create shaders.
  auto ps_blob =
      dx::ComputeShaderHelper::LoadShaderBlob(color_checker_pixel_shader_path_);

  com::ThrowIfFailed(
      device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr,
                                pixel_shader_.ReleaseAndGetAddressOf()));
}

void ColorCheckerLayer::CreateConstBuffer() {
  CheckerBoxs init_buffer_data = {};
  color_checker_buffer_ = dx::ComputeShaderHelper::CreateConstBuffer(
      init_buffer_data, D3D11_USAGE_DYNAMIC);
}
