#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <cstdint>

namespace dx {
class DeviceResources {
 public:
  static DeviceResources& GetSingleton();

  DeviceResources(const DeviceResources&) = delete;
  DeviceResources& operator=(const DeviceResources&) = delete;

  DeviceResources(DeviceResources&&) = delete;
  DeviceResources& operator=(DeviceResources&&) = delete;

  void CreateDeviceResources();
  void ClearDeviceResources();

  ID3D11Device1* GetDevice() const;
  ID3D11DeviceContext1* GetDeviceContext() const;
  D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const;

 private:
  DeviceResources() = default;
  ~DeviceResources() = default;

  Microsoft::WRL::ComPtr<ID3D11Device1> device_ = nullptr;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext1> immediate_context_ = nullptr;

  D3D_FEATURE_LEVEL feature_level_ = D3D_FEATURE_LEVEL_11_0;
};
}  // namespace dx
