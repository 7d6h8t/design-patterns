#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <cstdint>

namespace dx {
class RenderingResources final {
 public:
  RenderingResources() = default;
  ~RenderingResources() = default;

  RenderingResources(const RenderingResources&) = delete;
  RenderingResources& operator=(const RenderingResources&) = delete;

  RenderingResources(RenderingResources&&) = delete;
  RenderingResources& operator=(RenderingResources&&) = delete;

  void Present();

  void CreateBufferSizeDependentResources();

  void SetRenderingWindow(const HWND window);
  void SetBackBufferSize(const SIZE& size);

  IDXGISwapChain1* GetSwapChain() const;
  ID3D11RenderTargetView* GetRenderTargetView() const;
  DXGI_FORMAT GetBackBufferFormat() const;
  uint32_t GetBackBufferCount() const;
  SIZE GetBackBufferSize() const;
  HWND GetRenderingWindow() const;

 private:
  void CreateSwapChain();
  void CreateRenderTargetView();

  void HandleDeviceLost();

  Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_ = nullptr;

  inline const static DXGI_FORMAT back_buffer_format_ =
      DXGI_FORMAT_B8G8R8A8_UNORM;
  inline const static uint32_t back_buffer_count_ = 2;
  SIZE back_buffer_size_{};

  HWND rendering_window_ = nullptr;
};
}  // namespace dx
