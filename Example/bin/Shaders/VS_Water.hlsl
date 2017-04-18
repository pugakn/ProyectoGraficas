cbuffer ConstantBuffer{
    float4x4 WVP;
}

struct VS_INPUT{
    float4 position : POSITION;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float4 MVPPos : POSITION0;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
    OUT.MVPPos = OUT.hposition;
    return OUT;
}
