cbuffer ConstantBuffer{
    float4x4 WVP;
}

Texture2D TextureRGB : register(t0);

SamplerState SS;

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float4 MVPPos : POSITION0;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
  float2 projCoord = input.MVPPos.xy / input.MVPPos.w;
  float2 TexCoord = 0.5f * float2(projCoord.x,-projCoord.y) + 0.5;
  float4 reflectionColor = lerp(TextureRGB.Sample( SS, TexCoord.xy ), float4(0.1,0.2,0.6,1.0), 0.2);
  //float4 reflectionColor = TextureRGB.Sample( SS,projCoord );
  return reflectionColor;
}
