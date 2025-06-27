struct Vertex
{
    float4 position     : SV_POSITION;
    float2 texcoord     : TEXCOORD0;
};

struct Interpolants
{
    float4 position     : SV_POSITION;
    float2 texcoord     : TEXCOORD0;
};

Interpolants VS(Vertex input)
{
    return input;
}
