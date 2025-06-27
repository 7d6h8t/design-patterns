#pragma once
#include <filesystem>
#include <vector>
#include <array>

#include <utils.h>
#include "draw_layer.h"

class CoordinateProvider;
class ResourceProvider;

class ColorCheckerLayer : public DrawLayer {
 public:
  struct Box {
    capture_utils::PointF lt;
    capture_utils::PointF rt;
    capture_utils::PointF rb;
    capture_utils::PointF lb;
  };

  struct CheckerBoxs {
    Box check_box[24];
  };

  ColorCheckerLayer(CoordinateProvider& coordinate, ResourceProvider& resource);
  ~ColorCheckerLayer() = default;

  void Enable(bool enable);
  bool IsEnable() const;

  static std::vector<std::array<POINT, 4>> GetPhysicalCheckerBoxs();
  void SetCheckerBoxs(const std::vector<POINT>& checker_boxs);
  void Draw(const std::vector<std::array<POINT, 4>>& checker_boxs);
  void Draw();

 private:
  void CreatePixelShader() override;
  void CreateConstBuffer() override;

  Microsoft::WRL::ComPtr<ID3D11Buffer> color_checker_buffer_ = nullptr;

  inline const static std::filesystem::path color_checker_pixel_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"color_checker_pixel.cso"};

  inline static std::vector<std::array<POINT, 4>> checker_boxs_{};

  bool enable_ = false;
};
