#include "pch.h"
#include "yv12_converter.h"

#include "../include/com_utils.h"
#include "../compute_shader_helper.h"

using namespace conversion;
using Microsoft::WRL::ComPtr;

YV12Converter::YV12Converter() {
  convert_to_yuv444_shader_ =
      dx::ComputeShaderHelper::CreateComputeShader(compute_shader_path_);
}

YV12Converter::~YV12Converter() {}

Frame YV12Converter::GetFrame(const BYTE* frame_data, const int32_t pitch,
                              const SIZE& size) {
  size_t frame_size = pitch * (size.cy + (size.cy / 2));

  Frame frame{std::make_unique<BYTE[]>(frame_size), size, pitch};
  std::memcpy(frame.bits_.get(), frame_data, sizeof(BYTE) * frame_size);

  return frame;
}

void YV12Converter::SaveFrame(std::ofstream& file_stream, const Frame& frame) {
  uint32_t h = frame.size_.cy;

  // write y plane
  char* y_plane = reinterpret_cast<char*>(frame.bits_.get());
  for (uint32_t line = 0; line < h; ++line)
    file_stream.write(y_plane + (frame.pitch_ * line), frame.pitch_);

  // write v plane
  auto v_plane = y_plane + (frame.pitch_ * h);
  for (uint32_t line = 0; line < h / 2; ++line)
    file_stream.write(v_plane + (frame.pitch_ / 2 * line), frame.pitch_ / 2);

  // write u plane
  auto u_plane = v_plane + (frame.pitch_ / 2 * h / 2);
  for (uint32_t line = 0; line < h / 2; ++line)
    file_stream.write(u_plane + (frame.pitch_ / 2 * line), frame.pitch_ / 2);
}

std::vector<ComPtr<ID3D11ShaderResourceView>>
YV12Converter::CreateInputputShaderResource(const Frame& frame) {
  auto y_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      frame.bits_.get(), frame.size_, frame.pitch_, DXGI_FORMAT_R8_UINT,
      D3D11_SRV_DIMENSION_TEXTURE2D);

  const BYTE* v_bits = frame.bits_.get() + (frame.pitch_ * frame.size_.cy);
  auto v_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      v_bits, {frame.size_.cx / 2, frame.size_.cy / 2}, frame.pitch_ / 2,
      DXGI_FORMAT_R8_UINT, D3D11_SRV_DIMENSION_TEXTURE2D);

  const BYTE* u_bits = v_bits + (frame.pitch_ * frame.size_.cy / 4);
  auto u_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      u_bits, {frame.size_.cx / 2, frame.size_.cy / 2}, frame.pitch_ / 2,
      DXGI_FORMAT_R8_UINT, D3D11_SRV_DIMENSION_TEXTURE2D);

  return {y_srv, v_srv, u_srv};
}
