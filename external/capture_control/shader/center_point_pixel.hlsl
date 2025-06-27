struct Interpolants
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct Pixel
{
    float4 color    : SV_TARGET;
};

cbuffer Frame : register(b0)
{
    uint width_;
    uint height_;
    uint2 padding_;
};

Pixel PS(Interpolants input)
{
    float aspect_ratio = (float)width_ / (float)height_;
    Pixel output;

    float2 center = float2(width_ / 2.0 / width_, height_ / 2.0 / height_);

    float line_thickness  = 0.002;
    float2 line_thickness_aspect_adjusted  = float2(line_thickness / aspect_ratio, line_thickness);

    bool is_horizontal_line = (input.texcoord.y > center.y - line_thickness_aspect_adjusted.y && input.texcoord.y < center.y + line_thickness_aspect_adjusted.y);
    bool is_vertical_line = (input.texcoord.x > center.x - line_thickness_aspect_adjusted.x && input.texcoord.x < center.x + line_thickness_aspect_adjusted.x);

    bool is_crosshair = is_horizontal_line || is_vertical_line;

    if (is_crosshair)
    {
        output.color = float4(1.0, 0.0, 0.0, 0.3);
    }
    else
    {
        output.color = float4(0.0, 0.0, 0.0, 0.0);
    }
    return output;
}
