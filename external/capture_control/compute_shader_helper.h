#pragma once
#include <filesystem>
#include <wrl/client.h>

#include "device_resources.h"
#include "include/com_utils.h"
#include "frame_data.h"
#include "include/pixel_stats.h"

namespace dx {
class ComputeShaderHelper {
 public:
  static Microsoft::WRL::ComPtr<ID3D11ComputeShader> CreateComputeShader(
      const std::filesystem::path& shader_path);

  static std::vector<uint8_t> LoadShaderBlob(
      const std::filesystem::path& shader_path);

  static Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture2D(
      const DXGI_FORMAT& format, const uint32_t width, const uint32_t height,
      const uint32_t bind_flags, const BYTE* init_data = nullptr,
      const LONG pitch = 0);

  template <typename T = PixelStats>
  static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateStructedBuffer(
      const std::vector<T>& data) {
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.ByteWidth = static_cast<uint32_t>(data.size()) * sizeof(T);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags =
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    buffer_desc.StructureByteStride = sizeof(T);

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = data.data();

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
    com::ThrowIfFailed(
        dx::DeviceResources::GetSingleton().GetDevice()->CreateBuffer(
            &buffer_desc, &init_data, buffer.ReleaseAndGetAddressOf()));

    return buffer;
  }

  static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
  CreateShaderResourceView(
      const Microsoft::WRL::ComPtr<ID3D11Resource>& resource,
      const DXGI_FORMAT& format, const D3D_SRV_DIMENSION& srv_dimension);

  static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
  CreateShaderResourceView(const BYTE* init_data, const SIZE& size,
                           const int32_t pitch, const DXGI_FORMAT& format,
                           const D3D_SRV_DIMENSION& srv_dimension);

  static Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>
  CreateUnorderedAccessView(
      const Microsoft::WRL::ComPtr<ID3D11Resource>& resource,
      const DXGI_FORMAT& format, const D3D11_UAV_DIMENSION& uav_dimension);

  template <typename T>
  static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateConstBuffer(
      const T& const_buffer, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT) {
    D3D11_BUFFER_DESC buffer_desc = {};
    buffer_desc.Usage = usage;
    buffer_desc.ByteWidth = sizeof(const_buffer);
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.StructureByteStride = sizeof(T);

    if (usage == D3D11_USAGE_DYNAMIC)
      buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = &const_buffer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
    com::ThrowIfFailed(
        dx::DeviceResources::GetSingleton().GetDevice()->CreateBuffer(
            &buffer_desc, &init_data, buffer.ReleaseAndGetAddressOf()));

    return buffer;
  }

  static SIZE GetTexture2dUAVSize(
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& uav);

  static PixelStats CopyGPUBuffer2CPUPixelStats(
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& uav);

  static Frame CopyGPUTexture2D2CPUFrame(
      const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& uav);

 private:
  ComputeShaderHelper() = default;
  ~ComputeShaderHelper() = default;
};
}  // namespace dx
