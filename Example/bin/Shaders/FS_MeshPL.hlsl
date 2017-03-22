cbuffer ConstantBuffer{
    float4x4 World;
    float4x4 WVP;
    float4 light;
    float4 lightColor;
    #ifdef USE_SPECULAR
    float4 camPos;
    #endif
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
  float4 pixelPos : POSITION1;
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
  float3 norm = normalize(input.hnormal);
  float3 lightDir = normalize(light - input.pixelPos);
  float lightDist = length(input.pixelPos - light);
  float  light_mod = clamp(dot(norm,lightDir),0.0,1.0) ;
  #ifdef USING_ATENUATION
  light_mod = min(light_mod / ((lightDist * lightDist)/15000.0),light_mod );
  #endif
  #ifdef USE_SPECULAR
  float4 specularCol = lightColor;
  float3 RL = reflect(lightDir,norm);
  float3 eyeDir = normalize(camPos - input.pixelPos).xyz;
  float specular = pow(dot(-eyeDir,RL),50.0);
  #endif
  #else
    #ifndef USE_VERTEXLIGHTING
    float light_mod = 1.0;
    #endif
#endif

#ifdef USE_VERTEXLIGHTING
return color * 0.3 + color * input.light_mod * lightColor;
#else
    #ifdef USE_SPECULAR
    return color * 0.3 + color * light_mod * lightColor + specularCol* specular;
    #else
    return color * 0.3 + color * light_mod * lightColor;
    #endif
  #endif
}
