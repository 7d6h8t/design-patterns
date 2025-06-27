cbuffer Frame : register(b0)
{
    uint width_;
    uint height_;
    uint2 padding_;
};

Texture2D<uint4> yuy2_packed : register(t0);

// AYUV format (byte order == VUYA)
RWTexture2D<uint4> yuv444_AYUV : register(u0);

[numthreads(16, 16, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    uint x = pixel.x * 2; // 한 스레드는 두 개의 픽셀을 처리
    uint y = pixel.y;

    if (x >= width_ || y >= height_)
        return;

    uint4 yuyv = yuy2_packed.Load(int3(x / 2, y, 0));

    uint y0 = yuyv.x;
    uint u0 = yuyv.y;
    uint y1 = yuyv.z;
    uint v0 = yuyv.w;
    uint a = 255;

    yuv444_AYUV[uint2(x, y)] = uint4(v0, u0, y0, a);
    if (x + 1 < width_) {
        yuv444_AYUV[uint2(x + 1, y)] = uint4(v0, u0, y1 , a);
    }
}
