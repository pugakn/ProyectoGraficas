cbuffer ConstantBuffer{
    float4x4 WVP;
    float xOffset;
    float yOffset;
    float xSeparation;
    float ySeparation;
}
struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0 : TEXCOORD;
};

Texture2D TextureRGB : register(t0);
SamplerState SS;

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float2  textCord = float2(input.texture0.x + xOffset,input.texture0.y+ yOffset);
    float alpha = TextureRGB.Sample( SS, textCord ).r;
    float4 color = float4(1.0,0.0,0.0,1.0);
    color.a = alpha;
    return color;
}
