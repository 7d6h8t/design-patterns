#pragma once
#include <DirectXMath.h>

namespace dx {
struct Vertex {
  DirectX::XMFLOAT4 position;
  DirectX::XMFLOAT2 texcoord;
};

struct FrameBuffer {
  DirectX::XMUINT2 size;
  DirectX::XMUINT2 padding;
};

struct RoiRectBufferF {
  DirectX::XMFLOAT2 left_top_;
  DirectX::XMFLOAT2 right_bottom_;
};

struct RoiRectBuffer {
  DirectX::XMUINT2 left_top_;
  DirectX::XMUINT2 right_bottom_;
};
}  // namespace dx
