cbuffer ConstantBuffer{
    float4x4 W;
}
struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0 : TEXCOORD;
};

Texture2D TextureRGB : register(t0);
SamplerState SS;

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color = TextureRGB.Sample( SS, input.texture0);
    return color;
}
