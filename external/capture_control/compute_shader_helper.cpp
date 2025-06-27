#include "pch.h"
#include "compute_shader_helper.h"

#include <utils.h>

using Microsoft::WRL::ComPtr;
using namespace dx;

ComPtr<ID3D11ComputeShader> ComputeShaderHelper::CreateComputeShader(
    const std::filesystem::path& shader_path) {
  // Load and create shaders.
  auto cs_blob = LoadShaderBlob(shader_path);

  ComPtr<ID3D11ComputeShader> compute_shader = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateComputeShader(
          cs_blob.data(), cs_blob.size(), nullptr,
          compute_shader.ReleaseAndGetAddressOf()));

  return compute_shader;
}

ComPtr<ID3D11Texture2D> ComputeShaderHelper::CreateTexture2D(
    const DXGI_FORMAT& format, const uint32_t width, const uint32_t height,
    const uint32_t bind_flags, const BYTE* init_data, const LONG pitch) {
  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = format;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = bind_flags;

  D3D11_SUBRESOURCE_DATA init_data_desc = {};
  init_data_desc.pSysMem = init_data;
  init_data_desc.SysMemPitch = pitch;

  ComPtr<ID3D11Texture2D> texture = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateTexture2D(
          &desc, init_data ? &init_data_desc : nullptr,
          texture.ReleaseAndGetAddressOf()));

  return texture;
}

ComPtr<ID3D11ShaderResourceView> ComputeShaderHelper::CreateShaderResourceView(
    const ComPtr<ID3D11Resource>& resource, const DXGI_FORMAT& format,
    const D3D_SRV_DIMENSION& srv_dimension) {
  D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
  desc.Format = format;
  desc.ViewDimension = srv_dimension;
  desc.Texture2D.MostDetailedMip = 0;
  desc.Texture2D.MipLevels = 1;

  ComPtr<ID3D11ShaderResourceView> srv = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateShaderResourceView(
          resource.Get(), &desc, srv.ReleaseAndGetAddressOf()));

  return srv;
}

ComPtr<ID3D11ShaderResourceView> ComputeShaderHelper::CreateShaderResourceView(
    const BYTE* init_data, const SIZE& size, const int32_t pitch,
    const DXGI_FORMAT& format, const D3D_SRV_DIMENSION& srv_dimension) {
  auto texture = CreateTexture2D(format, size.cx, size.cy,
                                 D3D11_BIND_SHADER_RESOURCE, init_data, pitch);

  return CreateShaderResourceView(texture, format, srv_dimension);
}

ComPtr<ID3D11UnorderedAccessView>
ComputeShaderHelper::CreateUnorderedAccessView(
    const ComPtr<ID3D11Resource>& resource, const DXGI_FORMAT& format,
    const D3D11_UAV_DIMENSION& uav_dimension) {
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
  desc.Format = format;
  desc.ViewDimension = uav_dimension;
  if (uav_dimension == D3D11_SRV_DIMENSION_BUFFER) desc.Buffer.NumElements = 1;

  ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton()
          .GetDevice()
          ->CreateUnorderedAccessView(resource.Get(), &desc,
                                      uav.ReleaseAndGetAddressOf()));

  return uav;
}

SIZE ComputeShaderHelper::GetTexture2dUAVSize(
    const ComPtr<ID3D11UnorderedAccessView>& uav) {
  ComPtr<ID3D11Resource> gpu_resource = nullptr;
  uav->GetResource(gpu_resource.ReleaseAndGetAddressOf());

  D3D11_RESOURCE_DIMENSION resource_dimension{};
  gpu_resource->GetType(&resource_dimension);

  if (resource_dimension != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
    throw std::exception("get gpu texture2d to cpu frame error!");

  ComPtr<ID3D11Texture2D> gpu_texture = nullptr;
  com::ThrowIfFailed(gpu_resource.As(&gpu_texture));

  D3D11_TEXTURE2D_DESC texture_desc{};
  gpu_texture->GetDesc(&texture_desc);

  return {static_cast<long>(texture_desc.Width),
          static_cast<long>(texture_desc.Height)};
}

PixelStats ComputeShaderHelper::CopyGPUBuffer2CPUPixelStats(
    const ComPtr<ID3D11UnorderedAccessView>& uav) {
  // read back data (gpu -> cpu)
  ComPtr<ID3D11Resource> gpu_resource = nullptr;
  uav->GetResource(gpu_resource.ReleaseAndGetAddressOf());

  D3D11_RESOURCE_DIMENSION resource_dimension{};
  gpu_resource->GetType(&resource_dimension);

  if (resource_dimension != D3D11_RESOURCE_DIMENSION_BUFFER)
    throw std::exception("copy gpu structed buffer to cpu pixel stats error!");

  ComPtr<ID3D11Buffer> gpu_buffer = nullptr;
  com::ThrowIfFailed(gpu_resource.As(&gpu_buffer));

  D3D11_BUFFER_DESC buffer_desc{};
  gpu_buffer->GetDesc(&buffer_desc);

  D3D11_BUFFER_DESC staging_buffer_desc = buffer_desc;
  staging_buffer_desc.Usage = D3D11_USAGE_STAGING;
  staging_buffer_desc.ByteWidth = buffer_desc.ByteWidth;
  staging_buffer_desc.BindFlags = 0;
  staging_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

  ComPtr<ID3D11Buffer> staging_buffer = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateBuffer(
          &staging_buffer_desc, nullptr,
          staging_buffer.ReleaseAndGetAddressOf()));

  dx::DeviceResources::GetSingleton().GetDeviceContext()->CopyResource(
      staging_buffer.Get(), gpu_buffer.Get());

  D3D11_MAPPED_SUBRESOURCE mapped_resource{};
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDeviceContext()->Map(
          staging_buffer.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource));

  PixelStats pixel_stats{
      *(reinterpret_cast<uint32_t*>(mapped_resource.pData)),
      *(reinterpret_cast<uint32_t*>(mapped_resource.pData) + 1),
      *(reinterpret_cast<uint32_t*>(mapped_resource.pData) + 2),
  };

  dx::DeviceResources::GetSingleton().GetDeviceContext()->Unmap(
      staging_buffer.Get(), 0);

  return pixel_stats;
}

Frame ComputeShaderHelper::CopyGPUTexture2D2CPUFrame(
    const ComPtr<ID3D11UnorderedAccessView>& uav) {
  // read back data (gpu -> cpu)
  ComPtr<ID3D11Resource> gpu_resource = nullptr;
  uav->GetResource(gpu_resource.ReleaseAndGetAddressOf());

  D3D11_RESOURCE_DIMENSION resource_dimension{};
  gpu_resource->GetType(&resource_dimension);

  if (resource_dimension != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
    throw std::exception("copy gpu texture2d to cpu frame error!");

  ComPtr<ID3D11Texture2D> gpu_texture = nullptr;
  com::ThrowIfFailed(gpu_resource.As(&gpu_texture));

  D3D11_TEXTURE2D_DESC texture_desc{};
  gpu_texture->GetDesc(&texture_desc);

  D3D11_TEXTURE2D_DESC staging_texture_desc = texture_desc;
  staging_texture_desc.Usage = D3D11_USAGE_STAGING;
  staging_texture_desc.BindFlags = 0;
  staging_texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
  staging_texture_desc.MiscFlags = 0;

  ComPtr<ID3D11Texture2D> staging_texture = nullptr;
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDevice()->CreateTexture2D(
          &staging_texture_desc, nullptr,
          staging_texture.ReleaseAndGetAddressOf()));

  dx::DeviceResources::GetSingleton().GetDeviceContext()->CopyResource(
      staging_texture.Get(), gpu_texture.Get());

  D3D11_MAPPED_SUBRESOURCE mapped_resource{};
  com::ThrowIfFailed(
      dx::DeviceResources::GetSingleton().GetDeviceContext()->Map(
          staging_texture.Get(), 0, D3D11_MAP_READ, 0, &mapped_resource));

  size_t frame_size = mapped_resource.RowPitch * texture_desc.Height;

  Frame frame{std::make_unique<BYTE[]>(frame_size),
              {static_cast<long>(texture_desc.Width),
               static_cast<long>(texture_desc.Height)},
              static_cast<int32_t>(mapped_resource.RowPitch)};

  std::memcpy(frame.bits_.get(), mapped_resource.pData,
              sizeof(BYTE) * frame_size);

  dx::DeviceResources::GetSingleton().GetDeviceContext()->Unmap(
      staging_texture.Get(), 0);

  return frame;
}

std::vector<uint8_t> ComputeShaderHelper::LoadShaderBlob(
    const std::filesystem::path& shader_path) {
  std::basic_ifstream<uint8_t> file(shader_path,
                                    std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(size);
  file.read(buffer.data(), size);
  return buffer;
}
