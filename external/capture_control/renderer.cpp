#include "pch.h"
#include "renderer.h"

#include <mfidl.h>
#include <mftransform.h>
#include <mfapi.h>

#include "include/com_utils.h"
#include "dx_struct.h"
#include "compute_shader_helper.h"
#include "conversion/nv12_converter.h"
#include "conversion/yuy2_converter.h"
#include "conversion/i420_converter.h"
#include "conversion/yv12_converter.h"

using Microsoft::WRL::ComPtr;

Renderer::Renderer() noexcept {}

Renderer::~Renderer() noexcept {}

RECT Renderer::ScreenToPhysical(const RECT& screen) {
  return coordinate_.WindowScreenToPhysical(screen);
}

POINT Renderer::ScreenToPhysical(const POINT& screen) {
  return coordinate_.WindowScreenToPhysical(screen);
}

SIZE Renderer::GetFrameSize() const {
  return rendering_resources_.GetBackBufferSize();
}

HWND Renderer::GetRenderingWindow() const {
  return rendering_resources_.GetRenderingWindow();
}

void Renderer::Init(const HWND& wnd) {
  roi_rect_layer_.Init();
  center_point_layer_.Init();
  color_checker_layer_.Init();
  rendering_resources_.SetRenderingWindow(wnd);

  CreateVertexShader();
  CreatePixelShader();
  CreateVertexBuffer();
  CreateIndexBuffer();
  CreateSamplerState();
}

void Renderer::Render(const Frame& frame) {
  Clear();
  CreateRenderingSRV(frame);

  // Rendering Pipeline
  {
    auto immediate_context =
        dx::DeviceResources::GetSingleton().GetDeviceContext();

    // Input Assembler (IA)
    immediate_context->IASetInputLayout(vertex_layout_.Get());
    immediate_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    uint32_t strides = sizeof(dx::Vertex);
    uint32_t offsets = 0;
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(),
                                          &strides, &offsets);
    immediate_context->IASetIndexBuffer(index_buffer_.Get(),
                                        DXGI_FORMAT_R16_UINT, 0);

    // Vertex Shader (VS)
    immediate_context->VSSetShader(vertex_shader_.Get(), nullptr, 0);

    // Geometry Shader (GS)
    immediate_context->GSSetShader(nullptr, nullptr, 0);

    // Pixel Shader (PS)
    immediate_context->PSSetShader(pixel_shader_.Get(), nullptr, 0);
    immediate_context->PSSetSamplers(0, 1, sampler_state_.GetAddressOf());
    immediate_context->PSSetShaderResources(0, 1,
                                            rendering_srv_.GetAddressOf());

    immediate_context->DrawIndexed(6, 0, 0);
  }

  roi_rect_layer_.Draw();
  center_point_layer_.Draw();
  color_checker_layer_.Draw();
  rendering_resources_.Present();
}

void Renderer::AdjustViewport() {
  RECT screen_rect{};
  GetClientRect(rendering_resources_.GetRenderingWindow(), &screen_rect);

  viewport_.Init(GetFrameSize(), {RectUtils::GetWidth(screen_rect),
                             RectUtils::GetHeight(screen_rect)});
}

void Renderer::SetZoom(bool zoom_in, const POINT& screen_point) {
  viewport_.Zoom(zoom_in, screen_point);
}

void Renderer::SetViewportPosition(const POINT& last, const POINT& current) {
  viewport_.Move(last, current);
}

void Renderer::SetSize(const SIZE& frame_size) {
  rendering_resources_.SetBackBufferSize(frame_size);
  rendering_resources_.CreateBufferSizeDependentResources();
}

void Renderer::SetRoiScreenRect(const RECT& rect) {
  roi_rect_layer_.SetRoiScreenRect(rect);
}

void Renderer::SetRoiPhysicalRect(const RECT& rect) {
  roi_rect_layer_.SetRoiPhysicalRect(rect);
}

void Renderer::EnableCenterPoint(bool enable) {
  center_point_layer_.Enable(enable);
}

void Renderer::EnableRoiRect(bool enable) { roi_rect_layer_.Enable(enable); }

void Renderer::EnableColorChecker(bool enable) {
  color_checker_layer_.Enable(enable);
}

void Renderer::SetPhysicalCheckerBox(const std::vector<POINT>& checker_boxs) {
  color_checker_layer_.SetCheckerBoxs(checker_boxs);
}

ID3D11ShaderResourceView* Renderer::GetRenderingSRV() {
  return rendering_srv_.Get();
}

void Renderer::CreateVertexShader() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  // Load and create shaders.
  auto vs_blob = dx::ComputeShaderHelper::LoadShaderBlob(vertex_shader_path_);

  com::ThrowIfFailed(
      device->CreateVertexShader(vs_blob.data(), vs_blob.size(), nullptr,
                                 vertex_shader_.ReleaseAndGetAddressOf()));

  CreateInputLayout(vs_blob);
}

void Renderer::CreateInputLayout(const std::vector<uint8_t>& blob) {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  static const D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
      {"SV_POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
       D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16,
       D3D11_INPUT_PER_VERTEX_DATA, 0},
  };

  com::ThrowIfFailed(device->CreateInputLayout(
      input_element_desc, ARRAYSIZE(input_element_desc), blob.data(),
      blob.size(), vertex_layout_.ReleaseAndGetAddressOf()));
}

void Renderer::CreatePixelShader() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  // Load and create shaders.
  auto ps_blob = dx::ComputeShaderHelper::LoadShaderBlob(pixel_shader_path_);

  com::ThrowIfFailed(
      device->CreatePixelShader(ps_blob.data(), ps_blob.size(), nullptr,
                                pixel_shader_.ReleaseAndGetAddressOf()));
}

void Renderer::CreateVertexBuffer() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  static const dx::Vertex vertex_data[] = {
      {{-1.0f, -1.0f, 0.5f, 1.0f}, {0.f, 1.f}},
      {{1.0f, -1.0f, 0.5f, 1.0f}, {1.f, 1.f}},
      {{1.0f, 1.0f, 0.5f, 1.0f}, {1.f, 0.f}},
      {{-1.0f, 1.0f, 0.5f, 1.0f}, {0.f, 0.f}},
  };

  D3D11_SUBRESOURCE_DATA initial_data{};
  initial_data.pSysMem = vertex_data;

  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = sizeof(vertex_data);
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.StructureByteStride = sizeof(dx::Vertex);

  com::ThrowIfFailed(device->CreateBuffer(
      &desc, &initial_data, vertex_buffer_.ReleaseAndGetAddressOf()));
}

void Renderer::CreateIndexBuffer() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  static const uint16_t index_data[] = {
      3, 1, 0, 2, 1, 3,
  };

  D3D11_SUBRESOURCE_DATA initial_data{};
  initial_data.pSysMem = index_data;

  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = sizeof(index_data);
  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  desc.StructureByteStride = sizeof(uint16_t);

  com::ThrowIfFailed(device->CreateBuffer(
      &desc, &initial_data, index_buffer_.ReleaseAndGetAddressOf()));
}

void Renderer::CreateSamplerState() {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  D3D11_SAMPLER_DESC desc{};
  desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  desc.MinLOD = 0;
  desc.MaxLOD = D3D11_FLOAT32_MAX;

  com::ThrowIfFailed(
      device->CreateSamplerState(&desc, sampler_state_.GetAddressOf()));
}

void Renderer::CreateRenderingSRV(const Frame& frame) {
  auto device = dx::DeviceResources::GetSingleton().GetDevice();

  ComPtr<ID3D11Texture2D> texture = nullptr;
  {
    // create texture
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = frame.size_.cx;
    desc.Height = frame.size_.cy;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = rendering_resources_.GetBackBufferFormat();
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA init_data_desc = {};
    init_data_desc.pSysMem = frame.bits_.get();
    init_data_desc.SysMemPitch = frame.pitch_;

    com::ThrowIfFailed(device->CreateTexture2D(
        &desc, frame.bits_.get() ? &init_data_desc : nullptr,
        texture.ReleaseAndGetAddressOf()));
  }

  {
    // create srv
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Format = rendering_resources_.GetBackBufferFormat();
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MostDetailedMip = 0;
    desc.Texture2D.MipLevels = 1;

    com::ThrowIfFailed(device->CreateShaderResourceView(
        texture.Get(), &desc, rendering_srv_.ReleaseAndGetAddressOf()));
  }
}

void Renderer::Clear() {
  auto immediate_context =
      dx::DeviceResources::GetSingleton().GetDeviceContext();
  auto render_target_view = rendering_resources_.GetRenderTargetView();

  static auto black = DirectX::XMVECTORF32{0.0f, 0.0f, 0.0f, 1.0f};
  immediate_context->ClearRenderTargetView(render_target_view, black);

  // Output Merger (OM)
  immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);

  // Rasterizer (RS)
  immediate_context->RSSetViewports(1, &viewport_);
}
