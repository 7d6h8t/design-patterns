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

struct Box
{
    float2 lt;
    float2 rt;
    float2 rb;
    float2 lb;
};

cbuffer ColorChecker : register(b1)
{
    Box check_box[24];
};

bool IsValidBox(Box roi)
{
    if (roi.rb.x - roi.lt.x > 0 && roi.rb.y - roi.lt.y > 0)
        return true;
    else
        return false;
}

float2 DistanceToLine(float2 pos, float2 line_start, float2 line_end)
{
    float2 line_dir = line_end - line_start;          // 선분의 방향 벡터를 계산
    float2 point_dir = pos - line_start;              // 선분 시작점에서 점까지의 벡터를 계산
    float line_length = length(line_dir);             // 선분의 길이를 계산
    line_dir /= line_length;                          // 방향 벡터를 정규화

    float t = dot(point_dir, line_dir);               // point_dir을 line_dir에 투영하여 선분 위의 가장 가까운 점을 찾음
    t = clamp(t, 0.0, line_length);                   // t를 0.0과 line_length 사이로 제한하여 가장 가까운 점이 선분 내에 있도록 보장

    float2 closest_point = line_start + line_dir * t; // 선분 위의 가장 가까운 점의 좌표를 계산
    float2 distance = abs(pos - closest_point);
    return distance;
}

bool IsBoxBorder(float2 pos, Box roi, float2 border_thickness)
{
    float2 thickness = border_thickness / 2;
    // 선분에 대해 거리 계산
    float2 distance_lt_rt = DistanceToLine(pos, roi.lt, roi.rt);
    float2 distance_rt_rb = DistanceToLine(pos, roi.rt, roi.rb);
    float2 distance_rb_lb = DistanceToLine(pos, roi.rb, roi.lb);
    float2 distance_lb_lt = DistanceToLine(pos, roi.lb, roi.lt);

    // 각각의 변에 대해 x, y 방향 거리와 임계값 비교
    if ((distance_lt_rt.x <= thickness.x && distance_lt_rt.y <= thickness.y) ||
        (distance_rt_rb.x <= thickness.x && distance_rt_rb.y <= thickness.y) ||
        (distance_rb_lb.x <= thickness.x && distance_rb_lb.y <= thickness.y) ||
        (distance_lb_lt.x <= thickness.x && distance_lb_lt.y <= thickness.y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

float4 DrawColorChecker(float2 pos, float thickness, float4 line_color)
{
    float aspect_ratio = (float)width_ / (float)height_;

    float2 border_thickness_aspect_adjusted =
      float2(thickness / aspect_ratio, thickness);

    for (int i = 0; i < 24; ++i)
    {
        Box roi = check_box[i];
        if (IsValidBox(roi))
        {
            if (IsBoxBorder(pos, roi, border_thickness_aspect_adjusted))
            {
                return line_color;
            }
        }
    }
    return float4(0.0, 0.0, 0.0, 0.0);
}

Pixel PS(Interpolants input)
{
    Pixel output;

    float border_thickness = 0.003;
    float4 line_color = float4(0.0, 1.0, 0.0, 0.3);
    output.color = DrawColorChecker(input.texcoord, border_thickness, line_color);

    return output;
}
