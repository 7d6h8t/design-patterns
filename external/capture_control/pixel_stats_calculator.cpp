#include "pch.h"
#include "pixel_stats_calculator.h"
#include "compute_shader_helper.h"
#include "device_resources.h"
#include "dx_struct.h"

using Microsoft::WRL::ComPtr;

void PixelStatsCalculator::Calculate(
    const ComPtr<ID3D11UnorderedAccessView>& yuv444_uav,
    const ComPtr<ID3D11UnorderedAccessView>& rgba32_uav, const RECT& roi) {

  auto roi_width = roi.right - roi.left;
  auto roi_height = roi.bottom - roi.top;

  if (roi_width <= 1 || roi_height <= 1) return;

  auto immediate_context =
      dx::DeviceResources::GetSingleton().GetDeviceContext();

  auto size = dx::ComputeShaderHelper::GetTexture2dUAVSize(rgba32_uav);

  dx::FrameBuffer frame_buffer_data = {
      {static_cast<uint32_t>(size.cx), static_cast<uint32_t>(size.cy)}};
  auto const_buffer =
      dx::ComputeShaderHelper::CreateConstBuffer(frame_buffer_data);
  immediate_context->CSSetConstantBuffers(0, 1, const_buffer.GetAddressOf());

  dx::RoiRectBuffer roi_buffer_data = {
      {static_cast<uint32_t>(roi.left), static_cast<uint32_t>(roi.top)},
      {static_cast<uint32_t>(roi.right), static_cast<uint32_t>(roi.bottom)}};
  auto roi_buffer = dx::ComputeShaderHelper::CreateConstBuffer(roi_buffer_data);
  immediate_context->CSSetConstantBuffers(1, 1, roi_buffer.GetAddressOf());

  immediate_context->CSSetShader(pixel_calculator_shader_.Get(), nullptr, 0);
  immediate_context->CSSetUnorderedAccessViews(0, 1, yuv444_uav.GetAddressOf(),
                                               nullptr);
  immediate_context->CSSetUnorderedAccessViews(1, 1, rgba32_uav.GetAddressOf(),
                                               nullptr);

  auto y_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(2, 1, y_pixel_uav.GetAddressOf(),
                                               nullptr);

  auto u_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(3, 1, u_pixel_uav.GetAddressOf(),
                                               nullptr);

  auto v_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(4, 1, v_pixel_uav.GetAddressOf(),
                                               nullptr);

  auto r_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(5, 1, r_pixel_uav.GetAddressOf(),
                                               nullptr);

  auto g_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(6, 1, g_pixel_uav.GetAddressOf(),
                                               nullptr);

  auto b_pixel_uav = CreatePixelStatsUAV({initial_pixel_});
  immediate_context->CSSetUnorderedAccessViews(7, 1, b_pixel_uav.GetAddressOf(),
                                               nullptr);

  immediate_context->Dispatch((size.cx + 15) / 16, (size.cy + 15) / 16, 1);

  immediate_context->CSSetShader(nullptr, nullptr, 0);

  float pixel_count = static_cast<float>(roi_width * roi_height);

  auto y_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(y_pixel_uav);
  y_.avg_ = y_stats.sum_ / pixel_count;
  y_.min_ = y_stats.min_;
  y_.max_ = y_stats.max_;

  auto u_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(u_pixel_uav);
  u_.avg_ = u_stats.sum_ / pixel_count;
  u_.min_ = u_stats.min_;
  u_.max_ = u_stats.max_;

  auto v_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(v_pixel_uav);
  v_.avg_ = v_stats.sum_ / pixel_count;
  v_.min_ = v_stats.min_;
  v_.max_ = v_stats.max_;

  auto r_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(r_pixel_uav);
  r_.avg_ = r_stats.sum_ / pixel_count;
  r_.min_ = r_stats.min_;
  r_.max_ = r_stats.max_;

  auto g_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(g_pixel_uav);
  g_.avg_ = g_stats.sum_ / pixel_count;
  g_.min_ = g_stats.min_;
  g_.max_ = g_stats.max_;

  auto b_stats =
      dx::ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(b_pixel_uav);
  b_.avg_ = b_stats.sum_ / pixel_count;
  b_.min_ = b_stats.min_;
  b_.max_ = b_stats.max_;
}

PixelInfo PixelStatsCalculator::GetPixelInfoY() { return y_; }

PixelInfo PixelStatsCalculator::GetPixelInfoU() { return u_; }

PixelInfo PixelStatsCalculator::GetPixelInfoV() { return v_; }

PixelInfo PixelStatsCalculator::GetPixelInfoR() { return r_; }

PixelInfo PixelStatsCalculator::GetPixelInfoG() { return g_; }

PixelInfo PixelStatsCalculator::GetPixelInfoB() { return b_; }

ComPtr<ID3D11UnorderedAccessView> PixelStatsCalculator::CreatePixelStatsUAV(
    const std::vector<PixelStats>& pixel_stats) {
  auto buffer = dx::ComputeShaderHelper::CreateStructedBuffer(pixel_stats);

  D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = static_cast<uint32_t>(pixel_stats.size());

  ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton()
          .GetDevice()
          ->CreateUnorderedAccessView(buffer.Get(), &desc,
                                      uav.ReleaseAndGetAddressOf()));

  return uav;
}

PixelStatsCalculator::PixelStatsCalculator() {
  pixel_calculator_shader_ =
      dx::ComputeShaderHelper::CreateComputeShader(compute_shader_path_);
}
