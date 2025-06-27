#pragma once
#include <d3d11.h>
#include <utils.h>
#include <wrl/client.h>

#include <vector>
#include <filesystem>

#include "format_converter.h"

namespace conversion {
class I420Converter : public FormatConverter {
 public:
  I420Converter();
  ~I420Converter();

 private:
  Frame GetFrame(const BYTE* frame_data, const int32_t pitch,
                 const SIZE& size) override;

  void SaveFrame(std::ofstream& file_stream, const Frame& frame) override;

  std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
  CreateInputputShaderResource(const Frame& frame) override;

  inline const static std::filesystem::path compute_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"i420_to_yuv444.cso"};
};
}  // namespace conversion
