#ifdef G_DEFERRED_PASS
//uniform highp mat4 WVP;
//uniform highp mat4 World;
//uniform highp mat4 WorldView;
uniform highp mat4 VPInverse;
uniform highp vec4 LightPositions[128];
uniform highp vec4 LightColors[128];
uniform highp vec4 camPos;
uniform highp int NumLights;
uniform highp vec2 ShadowTexSize;

uniform mediump sampler2D texture01;
uniform mediump sampler2D texture04;
uniform mediump sampler2D texture03;
uniform mediump sampler2D texture02;

uniform highp vec3 linearLight;
uniform highp mat4 CamVP;
uniform mediump sampler2D texture05;


varying highp vec2 vecUVCoords;
//varying highp vec4 Pos;
varying highp vec4 PosCorner;
void main(){
	highp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	lowp vec4 Final  =  vec4(0.0,0.0,0.0,1.0);
	lowp vec4 color  =  texture2D(texture01,coords);
	color.w = 1.0;
	highp float depth  =	texture2D(texture02,coords).r;
	lowp vec4 Lambert = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 Specular = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 Ambient = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 normalmap = texture2D(texture04,coords);
	lowp vec4 normal = normalmap*2.0 - 1.0;
	normal	= normalize(normal);
	lowp vec4 specularmap = texture2D(texture03,coords);
	#ifndef LINEAR_DEPTH
	highp vec2 vcoord = coords *2.0 - 1.0;
	highp vec4 position = VPInverse*vec4(vcoord ,depth,1.0);
	position.xyz /= position.w;
	#else
	highp vec4 position = camPos + PosCorner*depth;
	#endif //LINEAR_DEPTH
	highp vec3  eyeDir   = normalize(camPos-position).xyz;
	highp float shinness = 4.0;
	shinness = normal.a + shinness;
	highp float attMax = 40.0;
	for(highp int i=0;i<NumLights;i++){
				highp float dist = distance(LightPositions[i],position);
				if(dist < (attMax*2)){
					Lambert  = LightColors[i];
					Specular = LightColors[i];

					lowp vec3 lightDir = normalize(LightPositions[i] - position).xyz;
					highp float att = 1.0;
					att = pow(dot(normal.xyz,lightDir) * 0.5 + 0.5,2.0);
					att = clamp( att,0.0,1.0) ;
					lowp vec3 RL = normalize(eyeDir+lightDir);
					highp float specular = dot(RL,normal.xyz) * 0.5 + 0.5;
					specular = pow( specular ,shinness);
					//att      =  min(att / ((dist * dist)/attMax),att );
					//specular =  min(specular / ((dist * dist)/attMax),specular);
					highp float d = max(dist - attMax, 0.0);
					highp float denom = d/attMax + 1.0;

					highp float attenuation = 1.0 / (denom*denom);
					attenuation = (attenuation - 0.8) / (1.0 - 0.8);
					attenuation = max(attenuation, 0.0);

					Lambert		*= color*att;
					Specular *= specular;
					Specular.xyz *= specularmap.xyz;


					Final += Lambert*attenuation ;
					Final += Specular*attenuation ;
				}
			}

			//Linear light
			highp float att = 1.0;
			//linearLight = normalize(linearLight);
			att = pow(dot(normal.xyz,linearLight) * 0.5 + 0.5,2.0);
			att = clamp( att,0.0,1.0) ;
			Final		+= color*att;
			//Linear Light End
			//Shadow Map=============================
			highp vec4 fromCamPos;
			position.w = 1.0;
			fromCamPos = CamVP*position;
			highp vec3 proj = fromCamPos.xyz / fromCamPos.w;
			proj = 0.5 * (proj + 1.0);
			highp vec2 shCoords =  proj.xy;
			highp float shadow = 0.0;
			if (shCoords.x <= 1 && shCoords.y <= 1 && shCoords.x >= 0 && shCoords.y >= 0
			&&proj.z <=1 && proj.z >= 0 ){
				highp vec2 texelSize =  1.0 / ShadowTexSize;
				for(int x = -1; x <= 1; ++x)
				{
				    for(int y = -1; y <= 1; ++y)
				    {
				        float pcfDepth = texture2D(texture05, shCoords + vec2(x,y) * texelSize).r;
								if (proj.z > pcfDepth +0.002)
								{
									//pixel en la sombra
									shadow += 1.0;
								}
				    }
				}
				shadow /= 9.0;
				Final.xyz = Final.xyz *(1.0-shadow);
			}
			//End Shadow Map ========================
			Ambient = color * 0.05;
			Final+= Ambient;
		//gl_FragColor = fromCamPos;
		//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
		//gl_FragColor = vec4(specularmap.xyz,1.0);
		gl_FragColor = vec4(Final.xyz,1.0);
}
#else //G_DEFERRED_PASS
uniform highp sampler2D texture01;
varying highp vec2 vecUVCoords;
void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	highp vec4 color = texture2D(texture01,coords);
	//color = vec4(depth,depth,depth,1.0);
	color.w = 1.0;
	gl_FragColor = color;
	//gl_FragColor = vec4(1.0,0,0,1.0);
}
#endif
