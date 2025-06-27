cbuffer Frame : register(b0)
{
    uint width_;
    uint height_;
    uint2 padding_;
};

Texture2D<uint> y_plane : register(t0);
Texture2D<uint> v_plane : register(t1);
Texture2D<uint> u_plane : register(t2);

// AYUV format (byte order == VUYA)
RWTexture2D<uint4> yuv444_AYUV : register(u0);

[numthreads(16, 16, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    if (pixel.x >= width_ || pixel.y >= height_)
        return;

    uint y = y_plane.Load(int3(pixel.xy, 0));
    uint v = v_plane.Load(int3(pixel.xy / 2, 0));
    uint u = u_plane.Load(int3(pixel.xy / 2, 0));
    uint a = 255;

    yuv444_AYUV[uint2(pixel.xy)] = uint4(v, u, y, a);
}
