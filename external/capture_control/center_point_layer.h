#pragma once
#include <filesystem>

#include <utils.h>
#include "draw_layer.h"

class CoordinateProvider;
class ResourceProvider;

class CenterPointLayer : public DrawLayer {
 public:
  CenterPointLayer(CoordinateProvider& coordinate, ResourceProvider& resource);
  ~CenterPointLayer() = default;

  void Enable(bool enable);
  bool IsEnable() const;
  void Draw();

 private:
  void CreatePixelShader() override;
  void CreateConstBuffer() override;

  inline const static std::filesystem::path center_point_pixel_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"center_point_pixel.cso"};

  bool enable_ = false;
};
