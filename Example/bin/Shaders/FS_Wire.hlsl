cbuffer ConstantBuffer{
    float4x4 WVP;
}
struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    return float4(1,1,1,1);
}
