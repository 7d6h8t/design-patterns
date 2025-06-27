#pragma once
#include "format_converter.h"

#include <vector>
#include <filesystem>
#include <wrl/client.h>
#include <d3d11.h>

#include <utils.h>

namespace conversion {
class NV12Converter : public FormatConverter {
 public:
  NV12Converter();
  ~NV12Converter();

 private:
  Frame GetFrame(const BYTE* frame_data, const int32_t pitch,
                 const SIZE& size) override;

  void SaveFrame(std::ofstream& file_stream, const Frame& frame) override;

  std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
  CreateInputputShaderResource(const Frame& frame) override;

  inline const static std::filesystem::path compute_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"nv12_to_yuv444.cso"};
};
}  // namespace conversion
