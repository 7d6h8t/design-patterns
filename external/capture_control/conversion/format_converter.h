#pragma once
#include <windef.h>
#include <vector>
#include <filesystem>
#include <wrl/client.h>

#include "../device_resources.h"
#include "../frame_data.h"
#include "../include/com_utils.h"
#include "../pixel_stats_calculator.h"

#include <utils.h>

struct IMFMediaBuffer;

namespace conversion {
class FormatConverter {
 public:
  virtual ~FormatConverter();

  Frame GetFormatedFrameData(
      const Microsoft::WRL::ComPtr<IMFMediaBuffer>& frame_buffer,
      const SIZE frame_size);

  Frame ConvertToRGB(const Frame& yuv_frame,
                     PixelStatsCalculator& pixel_calculator);
  void SaveFrame(const std::filesystem::path& path, const Frame& frame);
  void RecordFrame(const std::filesystem::path& path, const Frame& frame);

 protected:
  FormatConverter();

  virtual Frame GetFrame(const BYTE* frame_data, const int32_t pitch,
                         const SIZE& size) = 0;

  virtual void SaveFrame(std::ofstream& file_stream, const Frame& frame) = 0;

  virtual std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>
  CreateInputputShaderResource(const Frame& frame) = 0;

  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> ConvertToYUV444(
      const Frame& frame);

  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> YUV444ToRGBA32(
      const SIZE& size,
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& yuv444_uav);

  Microsoft::WRL::ComPtr<ID3D11ComputeShader> convert_to_yuv444_shader_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11ComputeShader> yuv444_to_rbga32_shader_ = nullptr;

  inline const static std::filesystem::path
      yuv444_to_rbga32_compute_shader_path_ = {
          utils::fileSystemLib::GetModuleDirectory() + L"yuv444_to_bgra32.cso"};

  inline const static uint32_t gpu_thread_group_x_ = 16;
  inline const static uint32_t gpu_thread_group_y_ = 16;
  inline const static uint32_t gpu_thread_group_z_ = 1;
};
}  // namespace conversion
