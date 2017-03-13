cbuffer ConstantBuffer{
    float4x4 World;
    float4x4 WVP;
    float4 lightDir;
}

Texture2D TextureRGB : register(t0);

SamplerState SS;

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

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color;

#ifdef USE_TEXCOORD0
	color = TextureRGB.Sample( SS, input.texture0 );
#else
	#ifdef USE_NORMALS
		color = normalize( input.hnormal );
	#else
		color = float4(0.5,0.5,0.5,1.0);
	#endif
#endif

#ifdef USE_PIXELLIGHTING
  float light_mod = clamp(dot(input.hnormal,lightDir)/(length(input.hnormal)*length(lightDir)),0.0,1.0) ;
  #else
    #ifndef USE_VERTEXLIGHTING
    float light_mod = 1.0;
    #endif
#endif

#ifdef USE_VERTEXLIGHTING
return color * input.light_mod;
#else
    return color * light_mod;
  #endif
}
