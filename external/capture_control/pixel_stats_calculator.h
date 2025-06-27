#pragma once
#include <filesystem>
#include <wrl/client.h>
#include <d3d11.h>

#include "include/pixel_stats.h"

#include <utils.h>

class PixelStatsCalculator {
 public:
  PixelStatsCalculator();
  ~PixelStatsCalculator() = default;

  PixelStatsCalculator(const PixelStatsCalculator&) = delete;
  PixelStatsCalculator& operator=(const PixelStatsCalculator&) = delete;

  PixelStatsCalculator(PixelStatsCalculator&&) = delete;
  PixelStatsCalculator& operator=(PixelStatsCalculator&&) = delete;

  void Calculate(
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& yuv444_uav,
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& rgba32_uav,
      const RECT& roi);

  PixelInfo GetPixelInfoY();
  PixelInfo GetPixelInfoU();
  PixelInfo GetPixelInfoV();
  PixelInfo GetPixelInfoR();
  PixelInfo GetPixelInfoG();
  PixelInfo GetPixelInfoB();
 private:
  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> CreatePixelStatsUAV(
      const std::vector<PixelStats>& pixel_stats);

  Microsoft::WRL::ComPtr<ID3D11ComputeShader> pixel_calculator_shader_ =
      nullptr;

  inline const static std::filesystem::path compute_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"pixel_calculator.cso"};

  inline const static PixelStats initial_pixel_ = {/*sum*/ 0, /*min*/ 255,
                                                   /*max*/ 0};

  PixelInfo y_{};
  PixelInfo u_{};
  PixelInfo v_{};

  PixelInfo r_{};
  PixelInfo g_{};
  PixelInfo b_{};
};
