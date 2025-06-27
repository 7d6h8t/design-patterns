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

cbuffer Roi : register(b1)
{
    float2 left_top_;
    float2 right_bottom_;
};

bool IsValidRect(float4 rect)
{
    float2 lt = float2(rect.x, rect.y);
    float2 rb = float2(rect.z, rect.w);
    if (rb.x - lt.x > 0 && rb.y - lt.y > 0)
        return true;
    else
        return false;
}

bool IsInRect(float2 pos, float4 rect)
{
    if (pos.x >= rect.x && pos.x <= rect.z &&
        pos.y >= rect.y && pos.y <= rect.w)
        return true;
    else
        return false;
}

bool IsBorder(float2 pos, float4 rect, float2 border_thickness)
{
    if (IsInRect(pos, rect))
    {
        if(pos.x < rect.x + border_thickness.x ||
           pos.x > rect.z - border_thickness.x ||
           pos.y < rect.y + border_thickness.y ||
           pos.y > rect.w - border_thickness.y)
            return true;
        else
            return false;
    }
    else
        return false;
}

float4 DrawRect(float2 pos, float thickness, float4 line_color)
{
    float aspect_ratio = (float)width_ / (float)height_;

    float2 border_thickness_aspect_adjusted =
      float2(thickness / aspect_ratio, thickness);

    float4 rect = float4(left_top_, right_bottom_);

    if (IsValidRect(rect))
    {
        if (IsBorder(pos, float4(left_top_, right_bottom_),
            border_thickness_aspect_adjusted))
            return line_color;
        else
            return float4(0.0, 0.0, 0.0, 0.0);
    }
    else
        return float4(0.0, 0.0, 0.0, 0.0);
}

Pixel PS(Interpolants input)
{
    Pixel output;

    float border_thickness = 0.003;
    float4 line_color = float4(1.0, 1.0, 0.0, 0.3);

    output.color = DrawRect(input.texcoord, border_thickness, line_color);
    return output;
}
