cbuffer ConstantBuffer{
    float4x4 WVP;
}

struct VS_INPUT{
    float4 position : POSITION;
    float2 texture0  : TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0 : TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.texture0 = input.texture0;
    OUT.hposition = mul(WVP,input.position);
    return OUT;
}
