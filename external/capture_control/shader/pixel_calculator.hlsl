cbuffer Frame : register(b0)
{
    uint width_;
    uint height_;
    uint2 padding_;
};

cbuffer Roi : register(b1)
{
    uint2 left_top_;
    uint2 right_bottom_;
};

// AYUV format (byte order == VUYA)
RWTexture2D<uint4> yuv444_AYUV_frame : register(u0);
RWTexture2D<uint4> bgra_frame : register(u1);

struct PixelStats {
    uint sum_;
    uint min_;
    uint max_;
};

RWStructuredBuffer<PixelStats> y_sum_min_max : register(u2);
RWStructuredBuffer<PixelStats> u_sum_min_max : register(u3);
RWStructuredBuffer<PixelStats> v_sum_min_max : register(u4);
RWStructuredBuffer<PixelStats> r_sum_min_max : register(u5);
RWStructuredBuffer<PixelStats> g_sum_min_max : register(u6);
RWStructuredBuffer<PixelStats> b_sum_min_max : register(u7);

[numthreads(16, 16, 1)]
void main(uint3 pixel : SV_DispatchThreadID)
{
    if (pixel.x >= width_ || pixel.y >= height_)
        return;

    // Check ROI
    if (pixel.x < left_top_.x || pixel.x >= right_bottom_.x ||
        pixel.y < left_top_.y || pixel.y >= right_bottom_.y)
        return;

    uint4 vuya = yuv444_AYUV_frame[uint2(pixel.xy)];
    uint y = vuya.z;
    uint u = vuya.y;
    uint v = vuya.x;

    uint4 bgra = bgra_frame[uint2(pixel.xy)];
    uint r = bgra.z;
    uint g = bgra.y;
    uint b = bgra.x;

    InterlockedAdd(y_sum_min_max[0].sum_, y);
    InterlockedAdd(u_sum_min_max[0].sum_, u);
    InterlockedAdd(v_sum_min_max[0].sum_, v);

    InterlockedAdd(r_sum_min_max[0].sum_, r);
    InterlockedAdd(g_sum_min_max[0].sum_, g);
    InterlockedAdd(b_sum_min_max[0].sum_, b);

    InterlockedMin(y_sum_min_max[0].min_, y);
    InterlockedMin(u_sum_min_max[0].min_, u);
    InterlockedMin(v_sum_min_max[0].min_, v);

    InterlockedMax(y_sum_min_max[0].max_, y);
    InterlockedMax(u_sum_min_max[0].max_, u);
    InterlockedMax(v_sum_min_max[0].max_, v);

    InterlockedMin(r_sum_min_max[0].min_, r);
    InterlockedMin(g_sum_min_max[0].min_, g);
    InterlockedMin(b_sum_min_max[0].min_, b);

    InterlockedMax(r_sum_min_max[0].max_, r);
    InterlockedMax(g_sum_min_max[0].max_, g);
    InterlockedMax(b_sum_min_max[0].max_, b);
}
