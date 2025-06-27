#include "pch.h"
#include "yuy2_converter.h"

#include "../include/com_utils.h"
#include "../compute_shader_helper.h"

using namespace conversion;
using Microsoft::WRL::ComPtr;

YUY2Converter::YUY2Converter() {
  convert_to_yuv444_shader_ =
      dx::ComputeShaderHelper::CreateComputeShader(compute_shader_path_);
}

YUY2Converter::~YUY2Converter() {}

Frame YUY2Converter::GetFrame(const BYTE* frame_data, const int32_t pitch,
                              const SIZE& size) {
  size_t frame_size = pitch * size.cy;

  Frame frame{std::make_unique<BYTE[]>(frame_size), size, pitch};
  std::memcpy(frame.bits_.get(), frame_data, sizeof(BYTE) * frame_size);

  return frame;
}

void YUY2Converter::SaveFrame(std::ofstream& file_stream, const Frame& frame) {
  uint32_t h = frame.size_.cy;
  char* yuv_data = reinterpret_cast<char*>(frame.bits_.get());
  
  file_stream.write(yuv_data, frame.pitch_ * h);
}

std::vector<ComPtr<ID3D11ShaderResourceView>>
YUY2Converter::CreateInputputShaderResource(const Frame& frame) {
  uint32_t width = frame.size_.cx;
  uint32_t height = frame.size_.cy;

  auto yuy2_texture = dx::ComputeShaderHelper::CreateTexture2D(
      DXGI_FORMAT_YUY2, width, height, D3D11_BIND_SHADER_RESOURCE,
      frame.bits_.get(), frame.pitch_);

  auto yuy2_srv = dx::ComputeShaderHelper::CreateShaderResourceView(
      yuy2_texture, DXGI_FORMAT_R8G8B8A8_UINT, D3D11_SRV_DIMENSION_TEXTURE2D);
  return {yuy2_srv};
}
