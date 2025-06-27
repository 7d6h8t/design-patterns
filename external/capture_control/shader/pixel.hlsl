struct Interpolants
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct Pixel
{
    float4 color    : SV_TARGET;
};

Texture2D diffuse_texture : register(t0);
SamplerState linear_sampler : register(s0);

Pixel PS(Interpolants input)
{
    Pixel output;
    output.color = diffuse_texture.Sample(linear_sampler, input.texcoord);
    return output;
}