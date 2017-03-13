cbuffer ConstantBuffer{
    float4x4 WVP;
	  float4x4 World;
    float4 lightDir;
}

struct VS_INPUT{
    float4 position : POSITION;

#ifdef USE_NORMALS
	float4 normal   : NORMAL;
#endif

#ifdef USE_TANGENTS
	float4 tangent  : TANGENT;
#endif

#ifdef USE_BINORMALS
	float4 binormal  : BINORMAL;
#endif

#ifdef USE_TEXCOORD0
    float2 texture0 : TEXCOORD;
#endif
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;

#ifdef USE_NORMALS
	float4 hnormal   : NORMAL;
#endif

#ifdef USE_TANGENTS
	float4 htangent   : TANGENT;
#endif

#ifdef USE_BINORMALS
	float4 hbinormal : BINORMAL;
#endif

#ifdef USE_TEXCOORD0
    float2 texture0  : TEXCOORD;
#endif

#ifdef USE_VERTEXLIGHTING
    float light_mod : PSIZE;
#endif

};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );

#ifdef USE_NORMALS
	OUT.hnormal = float4(normalize(mul( (float3x3)World , input.normal.xyz )),0.0);
#endif

#ifdef USE_TANGENTS
	OUT.htangent = normalize( mul( World , input.tangent ) );
#endif

#ifdef USE_BINORMALS
	OUT.hbinormal = normalize( mul( World , input.binormal ) );
#endif

#ifdef USE_TEXCOORD0
    OUT.texture0 = input.texture0;
#endif

#ifdef USE_VERTEXLIGHTING
    OUT.light_mod = clamp(dot(input.normal,lightDir)/(length(input.normal)*length(lightDir)),0.0,1.0) ;
#endif


    return OUT;
}
