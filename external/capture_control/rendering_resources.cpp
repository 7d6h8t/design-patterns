#include "pch.h"
#include "rendering_resources.h"

#include <format>
#include <exception>

#include <utils.h>

#include "include/com_utils.h"
#include "device_resources.h"

using namespace dx;
using Microsoft::WRL::ComPtr;

void RenderingResources::Present() {
  HRESULT hr = swap_chain_->Present(1, 0);
  dx::DeviceResources::GetSingleton().GetDeviceContext()->DiscardView(
      render_target_view_.Get());

  // If the device was removed either by a disconnection or a driver upgrade, we
  // must recreate all device resources.
  if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
#ifdef _DEBUG
    auto message = std::format("Device Lost on Present: Reason code 0x{:08X}",
                               (hr == DXGI_ERROR_DEVICE_REMOVED)
                                   ? dx::DeviceResources::GetSingleton()
                                         .GetDevice()
                                         ->GetDeviceRemovedReason()
                                   : hr);
    DebugPrint(message.c_str());
#endif
    HandleDeviceLost();
  } else {
    com::ThrowIfFailed(hr);
  }
}

void RenderingResources::CreateBufferSizeDependentResources() {
  if (rendering_window_ == nullptr)
    throw std::exception("Call SetWindow with a valid Win32 window handle");

  // Clear the previous window size specific context.
  ID3D11RenderTargetView* nullViews[] = {nullptr};
  dx::DeviceResources::GetSingleton().GetDeviceContext()->OMSetRenderTargets(
      _countof(nullViews), nullViews, nullptr);
  render_target_view_.Reset();
  dx::DeviceResources::GetSingleton().GetDeviceContext()->Flush();

  if (swap_chain_) {
    // If the swap chain already exists, resize it.
    HRESULT hr = swap_chain_->ResizeBuffers(
        back_buffer_count_, back_buffer_size_.cx, back_buffer_size_.cy,
        back_buffer_format_, 0);
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
#ifdef _DEBUG
      auto message =
          std::format("Device Lost on ResizeBuffers: Reason code 0x{:08X}",
                      (hr == DXGI_ERROR_DEVICE_REMOVED)
                          ? dx::DeviceResources::GetSingleton()
                                .GetDevice()
                                ->GetDeviceRemovedReason()
                          : hr);
      DebugPrint(message.c_str());
#endif
      // If the device was removed for any reason, a new device and swap chain
      // will need to be created.
      HandleDeviceLost();
      return;
    } else {
      com::ThrowIfFailed(hr);
    }
  } else {
    CreateSwapChain();
  }

  // Create a render target view of the swap chain back buffer.
  CreateRenderTargetView();
}

void RenderingResources::SetRenderingWindow(const HWND window) {
  rendering_window_ = window;
}

void RenderingResources::SetBackBufferSize(const SIZE& size) {
  back_buffer_size_ = size;
}

IDXGISwapChain1* RenderingResources::GetSwapChain() const {
  return swap_chain_.Get();
}

ID3D11RenderTargetView* RenderingResources::GetRenderTargetView() const {
  return render_target_view_.Get();
}

DXGI_FORMAT RenderingResources::GetBackBufferFormat() const {
  return back_buffer_format_;
}

uint32_t RenderingResources::GetBackBufferCount() const {
  return back_buffer_count_;
}

SIZE RenderingResources::GetBackBufferSize() const { return back_buffer_size_; }

HWND RenderingResources::GetRenderingWindow() const {
  return rendering_window_;
}

void RenderingResources::CreateSwapChain() {
  ComPtr<IDXGIFactory2> dxgi_factory = nullptr;
  com::ThrowIfFailed(
      CreateDXGIFactory1(IID_PPV_ARGS(dxgi_factory.ReleaseAndGetAddressOf())));

  // Create a descriptor for the swap chain.
  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};
  swap_chain_desc.Width = back_buffer_size_.cx;
  swap_chain_desc.Height = back_buffer_size_.cy;
  swap_chain_desc.Format = back_buffer_format_;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.BufferCount = back_buffer_count_;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = 0;
  swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  swap_chain_desc.Flags = 0;

  DXGI_SWAP_CHAIN_FULLSCREEN_DESC fs_swap_chain_desc{};
  fs_swap_chain_desc.Windowed = TRUE;

  // Create a SwapChain from a Win32 window.
  com::ThrowIfFailed(dxgi_factory->CreateSwapChainForHwnd(
      dx::DeviceResources::GetSingleton().GetDevice(), rendering_window_,
      &swap_chain_desc, &fs_swap_chain_desc, nullptr,
      swap_chain_.ReleaseAndGetAddressOf()));

  // This class does not support exclusive full-screen mode and prevents DXGI
  // from responding to the ALT+ENTER shortcut
  com::ThrowIfFailed(dxgi_factory->MakeWindowAssociation(
      rendering_window_, DXGI_MWA_NO_ALT_ENTER));
}

void RenderingResources::CreateRenderTargetView() {
  ComPtr<ID3D11Texture2D> back_buffer = nullptr;
  com::ThrowIfFailed(swap_chain_->GetBuffer(
      0, IID_PPV_ARGS(back_buffer.ReleaseAndGetAddressOf())));

  CD3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc(
      D3D11_RTV_DIMENSION_TEXTURE2D, back_buffer_format_);

  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateRenderTargetView(
          back_buffer.Get(), &render_target_view_desc,
          render_target_view_.ReleaseAndGetAddressOf()));
}

void RenderingResources::HandleDeviceLost() {
  render_target_view_.Reset();
  swap_chain_.Reset();

#ifdef _DEBUG
  com::CheckDXObjectsLeak();
#endif

  dx::DeviceResources::GetSingleton().ClearDeviceResources();
  dx::DeviceResources::GetSingleton().CreateDeviceResources();
  CreateBufferSizeDependentResources();
}
