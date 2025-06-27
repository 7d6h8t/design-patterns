#pragma once
#include <filesystem>

#include <utils.h>
#include "draw_layer.h"

class CoordinateProvider;
class ResourceProvider;

class ROIRectLayer : public DrawLayer {
 public:
  ROIRectLayer(CoordinateProvider& coordinate, ResourceProvider& resource);
  ~ROIRectLayer() = default;

  void Enable(bool enable);
  bool IsEnable() const;

  void SetRoiScreenRect(const RECT& rect);
  void SetRoiPhysicalRect(const RECT& rect);
  void Draw(const RECT& rect);
  void Draw();

  static RECT GetRoiPhysicalRect();

 private:
  void CreatePixelShader() override;
  void CreateConstBuffer() override;

  Microsoft::WRL::ComPtr<ID3D11Buffer> roi_buffer_ = nullptr;

  inline const static std::filesystem::path roi_rect_pixel_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"roi_rect_pixel.cso"};

  inline static RECT roi_rect_{};

  bool enable_ = false;
};
