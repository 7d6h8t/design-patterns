#include "pch.h"
#include "format_converter.h"

#include <mfobjects.h>

#include "../dx_struct.h"
#include "../compute_shader_helper.h"
#include "../pixel_stats_calculator.h"
#include "../roi_rect_layer.h"

#include <utils.h>

using namespace conversion;
using Microsoft::WRL::ComPtr;

FormatConverter::~FormatConverter() {}

Frame FormatConverter::GetFormatedFrameData(
    const ComPtr<IMFMediaBuffer>& frame_buffer, const SIZE frame_size) {
  ComPtr<IMF2DBuffer> frame_buffer_2d = nullptr;
  com::ThrowIfFailed(frame_buffer.As(&frame_buffer_2d));

  BYTE* input_data = nullptr;
  LONG pitch = 0;
  com::ThrowIfFailed(frame_buffer_2d->Lock2D(&input_data, &pitch));

  auto frame = GetFrame(input_data, pitch, frame_size);

  com::ThrowIfFailed(frame_buffer_2d->Unlock2D());

  return frame;
}

Frame FormatConverter::ConvertToRGB(const Frame& yuv_frame,
                                    PixelStatsCalculator& pixel_calculator) {
  auto yuv444_uav = ConvertToYUV444(yuv_frame);
  auto rgba32_uav = YUV444ToRGBA32(yuv_frame.size_, yuv444_uav);

  pixel_calculator.Calculate(yuv444_uav, rgba32_uav,
                             ROIRectLayer::GetRoiPhysicalRect());
  return dx::ComputeShaderHelper::CopyGPUTexture2D2CPUFrame(rgba32_uav);
}

void FormatConverter::SaveFrame(const std::filesystem::path& path,
                                const Frame& frame) {
  if (frame.bits_ == nullptr) throw std::exception("save error : data empty");

  std::ofstream file(path, std::ios::binary);
  if (!file.is_open()) throw std::exception("save error : file open fail");

  SaveFrame(file, frame);
}

void FormatConverter::RecordFrame(const std::filesystem::path& path,
                                  const Frame& frame) {
  if (frame.bits_ == nullptr) throw std::exception("record error : data empty");

  std::ofstream file(path, std::ios::binary | std::ios::app);
  if (!file.is_open()) throw std::exception("record error : file open fail");

  SaveFrame(file, frame);
}

FormatConverter::FormatConverter() {
  yuv444_to_rbga32_shader_ = dx::ComputeShaderHelper::CreateComputeShader(
      yuv444_to_rbga32_compute_shader_path_);
}

ComPtr<ID3D11UnorderedAccessView> FormatConverter::ConvertToYUV444(
    const Frame& frame) {
  dx::FrameBuffer frame_buffer_data = {{static_cast<uint32_t>(frame.size_.cx),
                                        static_cast<uint32_t>(frame.size_.cy)}};
  auto const_buffer =
      dx::ComputeShaderHelper::CreateConstBuffer(frame_buffer_data);

  auto srvs = CreateInputputShaderResource(frame);

  auto yuv444_texture = dx::ComputeShaderHelper::CreateTexture2D(
      DXGI_FORMAT_R8G8B8A8_UINT, frame.size_.cx, frame.size_.cy,
      D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);

  auto yuv444_uav = dx::ComputeShaderHelper::CreateUnorderedAccessView(
      yuv444_texture.Get(), DXGI_FORMAT_R8G8B8A8_UINT,
      D3D11_UAV_DIMENSION_TEXTURE2D);

  auto immediate_context =
      dx::DeviceResources::GetSingleton().GetDeviceContext();
  immediate_context->CSSetShader(convert_to_yuv444_shader_.Get(), nullptr, 0);
  immediate_context->CSSetConstantBuffers(0, 1, const_buffer.GetAddressOf());
  immediate_context->CSSetShaderResources(0, static_cast<uint32_t>(srvs.size()),
                                          srvs.data()->GetAddressOf());
  immediate_context->CSSetUnorderedAccessViews(0, 1, yuv444_uav.GetAddressOf(),
                                               nullptr);

  immediate_context->Dispatch(
      (frame.size_.cx + gpu_thread_group_x_ - 1) / gpu_thread_group_x_,
      (frame.size_.cy + gpu_thread_group_y_ - 1) / gpu_thread_group_y_,
      gpu_thread_group_z_);

  immediate_context->CSSetShader(nullptr, nullptr, 0);

  return yuv444_uav;
}

ComPtr<ID3D11UnorderedAccessView> FormatConverter::YUV444ToRGBA32(
    const SIZE& size, const ComPtr<ID3D11UnorderedAccessView>& yuv444_uav) {
  dx::FrameBuffer frame_buffer_data = {
      {static_cast<uint32_t>(size.cx), static_cast<uint32_t>(size.cy)}};
  auto const_buffer =
      dx::ComputeShaderHelper::CreateConstBuffer(frame_buffer_data);

  auto rgba32_texture = dx::ComputeShaderHelper::CreateTexture2D(
      DXGI_FORMAT_R8G8B8A8_UINT, size.cx, size.cy, D3D11_BIND_UNORDERED_ACCESS);

  auto rgba32_uav = dx::ComputeShaderHelper::CreateUnorderedAccessView(
      rgba32_texture.Get(), DXGI_FORMAT_R8G8B8A8_UINT,
      D3D11_UAV_DIMENSION_TEXTURE2D);

  auto immediate_context =
      dx::DeviceResources::GetSingleton().GetDeviceContext();
  immediate_context->CSSetShader(yuv444_to_rbga32_shader_.Get(), nullptr, 0);
  immediate_context->CSSetConstantBuffers(0, 1, const_buffer.GetAddressOf());
  immediate_context->CSSetUnorderedAccessViews(0, 1, yuv444_uav.GetAddressOf(), nullptr);
  immediate_context->CSSetUnorderedAccessViews(1, 1, rgba32_uav.GetAddressOf(),
                                               nullptr);

  immediate_context->Dispatch(
      (size.cx + gpu_thread_group_x_ - 1) / gpu_thread_group_x_,
      (size.cy + gpu_thread_group_y_ - 1) / gpu_thread_group_y_,
      gpu_thread_group_z_);

  immediate_context->CSSetShader(nullptr, nullptr, 0);

  return rgba32_uav;
}
