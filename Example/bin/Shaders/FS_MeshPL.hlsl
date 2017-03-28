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

#ifdef USE_SPEC_MAP
Texture2D TextureSpecular : register(t1);
SamplerState Sspec;
#endif

#ifdef USE_GLOSS_MAP
Texture2D TextureGloss : register(t2);
SamplerState Sgloss;
#endif

#ifdef USE_NORMAL_MAP
Texture2D TextureNormal : register(t3);
SamplerState Snor;
#endif


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

    #ifdef USE_TANGENTS
  	float3 tang = normalize(input.htangent.xyz);
  	#endif
  	#ifdef USE_BINORMALS
  	float3 binor = normalize(input.hbinormal.xyz);
  	#endif
  #ifdef USE_NORMAL_MAP
  	float3 norm = TextureNormal.Sample( Snor, input.texture0 ).xyz * 2.0 - 1.0;
  	norm.y = -norm.y;
  	float3x3 TBN = float3x3(tang,binor,normalize(input.hnormal.xyz));
  	norm = mul(norm ,TBN);
  	norm = normalize(norm);
    //float3 norm = normalize(input.hnormal.xyz);
  #else
  	float3 norm = normalize(input.hnormal.xyz);
  #endif

//#ifdef USE_GLOSS_MAP
//return TextureGloss.Sample( Sgloss, input.texture0 );
//#endif

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
  //float3 norm = normalize(input.hnormal);
  float3 lightDir = normalize(light - input.pixelPos);
  float lightDist = length(input.pixelPos - light);
  //float att = dot(norm,lightDir);
  float att = pow(dot(norm,lightDir) * 0.5 + 0.5,2.0);
  float  light_mod = clamp(att,0.0,1.0) ;

  #ifdef USE_SPECULAR
  float4 Specular = lightColor;

  float3 eyeDir = normalize(camPos - input.pixelPos);
  float3 RL = normalize(eyeDir+lightDir);
  float specular = dot(RL,norm) * 0.5 + 0.5;
  #ifdef USE_GLOSS_MAP
      specular = pow( specular ,TextureGloss.Sample( Sgloss, input.texture0 ));
      //specular = pow( specular ,specExp);
      #else
      specular = pow( specular ,10.0);
  #endif
  #endif
  #else
    #ifndef USE_VERTEXLIGHTING
    float light_mod = 1.0;
    #endif
#endif
#ifdef USING_ATENUATION
light_mod = min(light_mod / ((lightDist * lightDist)/15000.0),light_mod );
specular = min(specular / ((lightDist * lightDist)/15000.0),specular);
#endif


#ifdef USE_VERTEXLIGHTING
return color * 0.3 + color * input.light_mod * lightColor;
#else
    #ifdef USE_SPECULAR
    Specular *= specular;
    Specular *= att;
    #ifdef USE_SPEC_MAP
    Specular *= TextureSpecular.Sample( Sspec, input.texture0 );
    #endif
    return color * 0.3 + color * light_mod * lightColor + Specular;
    #else
    return color * 0.3 + color * light_mod * lightColor;
    #endif
  #endif
}
