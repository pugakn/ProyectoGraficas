
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

Texture2D difuse : register(t0);
Texture2D normalText: register(t1);
Texture2D specularText: register(t2);
Texture2D depthText: register(t3);
Texture2D shadowMap: register(t4);
SamplerState SS;

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
	float2 coords = input.texture0;
	//coords.y = 1.0 - coords.y;
	float4 Final  =  float4(0.0,0.0,0.0,1.0);
	float4 color  =  difuse.Sample( SS, coords);
	color.w = 1.0;
	 float depth  =	depthText.Sample(SS,coords).r;
	 float4 Lambert = float4(0.0,0.0,0.0,1.0);
	 float4 Specular = float4(0.0,0.0,0.0,1.0);
	 float4 Ambient = float4(0.0,0.0,0.0,1.0);
	 float4 normalmap = normalText.Sample(SS,coords);
	 float4 normal = normalmap*2.0 - 1.0;
	normal	= normalize(normal);
	 float4 specularmap = specularText.Sample(SS,coords);
	#ifndef LINEAR_DEPTH
	 
	 float4 position = mul(VPInverse,float4( input.PosCorner.xy ,depth,1.0));
	 position.xyz /= position.w;
	 position.w = 1;
	 
	#else
	 float4 position = CameraPosition + input.PosCorner*depth;
	#endif //LINEAR_DEPTH
	 float3  eyeDir   = normalize(CameraPosition-position).xyz;
	 float shinness = 8.0;
	shinness = shinness;//normal.a + shinness;
	 float attMax = 40.0;
	for( int i=0;i<NumLights2;i++){
				 float dist = distance(LightPositions[i],position);
				if(dist < (attMax*2)){
					Lambert  = LightColors[i];
					Specular = LightColors[i];

					 float3 lightDir = normalize(LightPositions[i] - position).xyz;
					 float att = 1.0;
					att = pow(dot(normal.xyz,lightDir) * 0.5 + 0.5,2.0);
					att = clamp( att,0.0,1.0) ;
					 float RL = normalize(eyeDir+lightDir);
					
					float specular = max ( dot(RL,normal.xyz)*0.5 + 0.5, 0.0);	
					specular = pow( specular ,shinness);	
					
					//att      =  min(att / ((dist * dist)/attMax),att );
					//specular =  min(specular / ((dist * dist)/attMax),specular);
					 float d = max(dist - attMax, 0.0);
					 float denom = d/attMax + 1.0;

					 float attenuation = 1.0 / (denom*denom);
					attenuation = (attenuation - 0.8) / (1.0 - 0.8);
					attenuation = max(attenuation, 0.0);

					Lambert		*= color*att;
					Specular *= specular;
					Specular.xyz *= specularmap.xyz;


					Final += Lambert*attenuation ;
					Final += Specular*attenuation ;
				}
			}
			Ambient = color * 0.1;
			Final+= Ambient;
			//Shadow Map=============================
			float4 fromCamPos;
			//position.w = 1.0;
			//float4 p  = float4(position.x,position.y-1,position.z,position.w);
			//fromCamPos = mul(CamVP,p);
		
			

		
			
			fromCamPos = mul(CamVP,position);
			
			
			float3 proj = fromCamPos.xyz;
			proj.xyz = proj.xyz /fromCamPos.w;
			proj.xy = 0.5 * (proj.xy + 1.0);
			float2 shCoords =  proj.xy;
			float shadow = 0.0;
			shCoords.y = 1-shCoords.y;
			if (shCoords.x <= 1 && shCoords.y <= 1 && shCoords.x >= 0 && shCoords.y >= 0
			&&proj.z <=1 && proj.z >= 0 ){
				float2 texelSize =  float2(1.0,1.0) / ShadowTexSize;
				for(int x = -1; x <= 1; ++x)
				{
				    for(int y = -1; y <= 1; ++y)
				    {
				        float pcfDepth = shadowMap.Sample(SS, shCoords + (float2(x,y) * texelSize)).r;
								if (proj.z > pcfDepth+ 0.02)
								{
									//pixel en la sombra
									shadow += 1.0;
									return float4(0,0,0,1);
								}
				    }
				}
				shadow /= 9.0;
				Final.xyz = Final.xyz *(1.0-shadow);
			}
			//End Shadow Map ========================
			//return float4(shadowMap.Sample(SS, coords).r,shadowMap.Sample(SS, coords).r,shadowMap.Sample(SS, coords).r,1);
		//return shadowMap;
		return float4(Final.xyz,1.0);
}
#else //G_DEFERRED_PASS


cbuffer ConstantBuffer{
    float4x4 W;
}

Texture2D TextureRGB : register(t0);
SamplerState SS;

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color = TextureRGB.Sample( SS, input.texture0);
    return color;
}

#endif
