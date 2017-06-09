cbuffer ConstantBuffer{
    float4x4 WorldView;
    float4x4 WVP;
	  float4x4 World;
    float4 light;
    float4 lightColor;
    #ifdef USE_SPECULAR
    float4 camPos;
    #endif
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

  float4 pixelPos : POSITION1;
  float4 Pos		: TEXCOORD1;

};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
    OUT.pixelPos = mul( World , input.position);
    OUT.Pos = mul( WorldView , input.position );

#ifdef USE_NORMALS
	OUT.hnormal = float4(normalize(mul( (float3x3)World , input.normal.xyz )),0.0);
#endif

#ifdef USE_TANGENTS
	OUT.htangent = float4(normalize( mul((float3x3) World , input.tangent.xyz ) ),0.0);
#endif

#ifdef USE_BINORMALS
	OUT.hbinormal = float4(normalize( mul((float3x3) World , input.binormal.xyz ) ),0.0);
#endif

#ifdef USE_TEXCOORD0
    OUT.texture0 = input.texture0;
#endif

#ifdef USE_VERTEXLIGHTING
  float3 norm = normalize(mul((float3x3)World , input.normal.xyz ));
  float3 lightDir = normalize(light - mul( World , input.position));
  float lightDist = length(mul( World , input.position) - light);
  OUT.light_mod = clamp(dot(norm,lightDir),0.0,1.0) ;
  #ifdef USING_ATENUATION
  OUT.light_mod = min(OUT.light_mod / ((lightDist * lightDist)/15000.0),OUT.light_mod );
  #endif
#endif


    return OUT;
}
