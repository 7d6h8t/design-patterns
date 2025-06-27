#include "pch.h"
#include "device_resources.h"

#include <format>
#include <exception>

#include <utils.h>

#include "include/com_utils.h"

using namespace dx;
using Microsoft::WRL::ComPtr;

DeviceResources& DeviceResources::GetSingleton() {
  static DeviceResources instance;
  return instance;
}

void DeviceResources::CreateDeviceResources() {
  uint32_t device_flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
  // ref :
  // https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-layers#debug-layer
  device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  static const D3D_FEATURE_LEVEL feature_levels[] = {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
  };

  ComPtr<ID3D11Device> device = nullptr;
  ComPtr<ID3D11DeviceContext> device_context = nullptr;

  com::ThrowIfFailed(D3D11CreateDevice(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, device_flag, feature_levels,
      _countof(feature_levels), D3D11_SDK_VERSION, device.GetAddressOf(),
      &feature_level_, device_context.GetAddressOf()));

#ifndef NDEBUG
  ComPtr<ID3D11Debug> d3d_debug = nullptr;
  if (SUCCEEDED(device.As(&d3d_debug))) {
    ComPtr<ID3D11InfoQueue> d3d_info_queue = nullptr;
    if (SUCCEEDED(d3d_debug.As(&d3d_info_queue))) {
#ifdef _DEBUG
      d3d_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION,
                                         true);
      d3d_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
      D3D11_MESSAGE_ID hide[] = {
          D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
      };
      D3D11_INFO_QUEUE_FILTER filter{};
      filter.DenyList.NumIDs = _countof(hide);
      filter.DenyList.pIDList = hide;
      d3d_info_queue->AddStorageFilterEntries(&filter);
    }
  }
#endif

  com::ThrowIfFailed(device.As(&device_));
  com::ThrowIfFailed(device_context.As(&immediate_context_));
}

void DeviceResources::ClearDeviceResources() {
  immediate_context_.Reset();
  device_.Reset();
}

ID3D11Device1* DeviceResources::GetDevice() const { return device_.Get(); }

ID3D11DeviceContext1* DeviceResources::GetDeviceContext() const {
  return immediate_context_.Get();
}

D3D_FEATURE_LEVEL DeviceResources::GetDeviceFeatureLevel() const {
  return feature_level_;
}
