cbuffer Frame : register(b0)
{
    uint width_;
    uint height_;
    uint2 padding_;
};

// AYUV format (byte order == VUYA)
RWTexture2D<uint4> yuv444_AYUV_frame : register(u0);
RWTexture2D<uint4> bgra_frame : register(u1);

uint Clip0To255(int value)
{
    return min(max(value, 0), 255);
}

float3x3 GetConversionCoefficients()
{
    // ref.1 : https://learn.microsoft.com/en-us/windows/win32/medfound/recommended-8-bit-yuv-formats-for-video-rendering
    // ref.2 : https://en.wikipedia.org/wiki/YCbCr
    if (width_ <= 720)  // SD 해상도 (BT.601)
    {
        return float3x3(
            1.164383, 0.0, 1.596027,
            1.164383, -0.391762, -0.812968,
            1.164383, 2.017232, 0.0
        );
    }
    else if (width_ <= 1920)  // HD 해상도 (BT.709)
    {
        return float3x3(
            1.0, 0.0, 1.5748,
            1.0, -0.1873, -0.4681,
            1.0, 1.8556, 0.0
        );
    }
    else  // UHD 해상도 (BT.2020)
    {
        return float3x3(
            1.0, 0.0, 1.4746,
            1.0, -0.16455312684366, -0.57135312684366,
            1.0, 1.8814, 0.0
        );
    }
}

uint3 YUV2RGB(uint3 yuv)
{
    // 1. y [16~235], uv[16~240] range -> c = yuv.x - 16;
    // 2. yuv [0~255] range -> c = yuv.x - 0;
    int c = yuv.x; // apply 2
    int d = yuv.y - 128;
    int e = yuv.z - 128;

    float3x3 coefficients = GetConversionCoefficients();

    int3 rgb = round(mul(coefficients, float3(c, d, e)));

    return uint3(Clip0To255(rgb.r), Clip0To255(rgb.g), Clip0To255(rgb.b));
}

[numthreads(16, 16, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    if (pixel.x >= width_ || pixel.y >= height_)
        return;

    uint4 vuya = yuv444_AYUV_frame[uint2(pixel.xy)];
    uint y = vuya.z;
    uint u = vuya.y;
    uint v = vuya.x;
    uint a = vuya.w;

    uint3 rgb = YUV2RGB(uint3(y, u, v));

    bgra_frame[uint2(pixel.xy)] = uint4(rgb.b, rgb.g, rgb.r, a);
}
