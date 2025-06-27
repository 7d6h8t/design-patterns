#include "pch.h"
#include "nv12_converter.h"

#include "../include/com_utils.h"
#include "../compute_shader_helper.h"

using namespace conversion;
using Microsoft::WRL::ComPtr;

NV12Converter::NV12Converter() {
  convert_to_yuv444_shader_ =
      dx::ComputeShaderHelper::CreateComputeShader(compute_shader_path_);
}

NV12Converter::~NV12Converter() {}

Frame NV12Converter::GetFrame(const BYTE* frame_data, const int32_t pitch,
                              const SIZE& size) {
  size_t frame_size = pitch * (size.cy + (size.cy / 2));

  Frame frame{std::make_unique<BYTE[]>(frame_size), size, pitch};
  std::memcpy(frame.bits_.get(), frame_data, sizeof(BYTE) * frame_size);

  return frame;
}

void NV12Converter::SaveFrame(std::ofstream& file_stream, const Frame& frame) {
  uint32_t h = frame.size_.cy;

  // write y plane
  char* y_plane = reinterpret_cast<char*>(frame.bits_.get());
  for (uint32_t line = 0; line < h; ++line)
    file_stream.write(y_plane + (frame.pitch_ * line), frame.pitch_);

  // write uv plane
  auto uv_plane = y_plane + (frame.pitch_ * h);
  for (uint32_t line = 0; line < h / 2; ++line)
    file_stream.write(uv_plane + (frame.pitch_ * line), frame.pitch_);
}

std::vector<ComPtr<ID3D11ShaderResourceView>>
NV12Converter::CreateInputputShaderResource(const Frame& frame) {
  auto y_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      frame.bits_.get(), frame.size_, frame.pitch_, DXGI_FORMAT_R8_UINT,
      D3D11_SRV_DIMENSION_TEXTURE2D);

  const BYTE* uv_bits = frame.bits_.get() + (frame.pitch_ * frame.size_.cy);
  auto uv_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      uv_bits, {frame.size_.cx / 2, frame.size_.cy / 2}, frame.pitch_,
      DXGI_FORMAT_R8G8_UINT, D3D11_SRV_DIMENSION_TEXTURE2D);

  return {y_srv, uv_srv};
}
