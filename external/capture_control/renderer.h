#pragma once
#include <windef.h>
#include <wrl/client.h>
#include <mfobjects.h>
#include <dxgiformat.h>
#include <d3d11.h>

#include <filesystem>
#include <vector>

#include <utils.h>

#include "coordinate_provider.h"
#include "resource_provider.h"
#include "rendering_resources.h"
#include "coordinate_system.h"
#include "roi_rect_layer.h"
#include "center_point_layer.h"
#include "color_checker_layer.h"
#include "frame_data.h"
#include "zoom_viewport.h"

struct IMFMediaBuffer;

class Renderer final : public CoordinateProvider, public ResourceProvider {
 public:
  Renderer() noexcept;
  ~Renderer() noexcept;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  RECT ScreenToPhysical(const RECT& screen) override;
  POINT ScreenToPhysical(const POINT& screen);
  SIZE GetFrameSize() const override;
  HWND GetRenderingWindow() const override;

  void Init(const HWND& wnd);
  void Render(const Frame& frame);

  void AdjustViewport();
  void SetZoom(bool zoom_in, const POINT& screen_point);
  void SetViewportPosition(const POINT& last, const POINT& current);
  void SetSize(const SIZE& frame_size);
  void SetRoiScreenRect(const RECT& rect);
  void SetRoiPhysicalRect(const RECT& rect);
  void SetPhysicalCheckerBox(const std::vector<POINT>& checker_boxs);

  void EnableCenterPoint(bool enable);
  void EnableRoiRect(bool enable);
  void EnableColorChecker(bool enable);

  ID3D11ShaderResourceView* GetRenderingSRV();

 private:
  void CreateVertexShader();
  void CreateInputLayout(const std::vector<uint8_t>& blob);
  void CreatePixelShader();
  void CreateVertexBuffer();
  void CreateIndexBuffer();
  void CreateSamplerState();
  void CreateRenderingSRV(const Frame& frame);

  void Clear();

  dx::RenderingResources rendering_resources_;
  CoordinateSystem coordinate_{};
  ZoomViewport viewport_{coordinate_, *this};
  ROIRectLayer roi_rect_layer_{*this, *this};
  CenterPointLayer center_point_layer_{*this, *this};
  ColorCheckerLayer color_checker_layer_{*this, *this};

  Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> vertex_layout_ = nullptr;

  Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rendering_srv_ = nullptr;

  inline const static std::filesystem::path vertex_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"vertex.cso"};

  inline const static std::filesystem::path pixel_shader_path_ = {
      utils::fileSystemLib::GetModuleDirectory() + L"pixel.cso"};
};
