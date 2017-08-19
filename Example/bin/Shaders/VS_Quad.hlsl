
struct VS_INPUT{
    float4 position : POSITION;
    float2 texture0  : TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0 : TEXCOORD;
	#ifdef G_DEFERRED_PASS
	float4 PosCorner : POSITION1;
	#endif
};
#ifdef G_DEFERRED_PASS
cbuffer ConstantBuffer{
	 float4x4 W;
	 float4x4 VPInverse;
	 float4x4 CamVP;
	 float4 LightPositions[128];
	 float4 LightColors[128];
	 float4 CameraPosition;
	 int NumLights;
	 float2 ShadowTexSize;
	 int NumLights2;
}
VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.texture0 = input.texture0;
    OUT.hposition = mul(W,input.position);
	//OUT.PosCorner = mul(VPInverse,float4(input.position.xy,1.0,1.0));
	//OUT.PosCorner.xyz /= OUT.PosCorner.w;
	//OUT.PosCorner = OUT.PosCorner - CameraPosition;
	
	OUT.PosCorner = float4(input.position.xy,1.0,1.0);
	
	//OUT.PosCorner = mul(VPInverse, float4(input.position.xy,1.0,1.0));
	//OUT.PosCorner.xyz /= OUT.PosCorner.w;
	//OUT.PosCorner = OUT.PosCorner - CameraPosition;
	
    return OUT;
}

#else
cbuffer ConstantBuffer{
    float4x4 W;
}

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.texture0 = input.texture0;
    OUT.hposition = mul(W,input.position);
    return OUT;
}
#endif
