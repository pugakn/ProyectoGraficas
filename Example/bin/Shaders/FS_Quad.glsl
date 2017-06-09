#ifdef G_BUFF_PASS
uniform highp mat4 WVP;
uniform highp mat4 World;
uniform highp mat4 WorldView;
uniform highp mat4 VPInverse;
uniform highp vec4 LightPositions[128];
uniform highp vec4 LightColors[128];
uniform highp vec4 CameraPosition;
uniform highp int NumLights;

uniform mediump sampler2D difuse;
uniform mediump sampler2D normalText;
uniform mediump sampler2D specularText;
uniform mediump sampler2D depthText;

varying highp vec2 vecUVCoords;
//varying highp vec4 Pos;
varying highp vec4 PosCorner;
void main(){
	highp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	lowp vec4 Final  =  vec4(0.0,0.0,0.0,1.0);
	lowp vec4 color  =  texture2D(difuse,coords);
	color.w = 1.0;
	highp float depth  =	texture2D(depthText,coords).r;
	lowp vec4 Lambert = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 Specular = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 Ambient = vec4(0.0,0.0,0.0,1.0);
	lowp vec4 normalmap = texture2D(normalText,coords);
	lowp vec4 normal = normalmap*2.0 - 1.0;
	normal	= normalize(normal);
	lowp vec4 specularmap = texture2D(specularText,coords);
	#ifndef LINEAR_DEPTH
	highp vec2 vcoord = coords *2.0 - 1.0;
	highp vec4 position = VPInverse*vec4(vcoord ,depth,1.0);
	position.xyz /= position.w;
	#else
	highp vec4 position = CameraPosition + PosCorner*depth;
	#endif //LINEAR_DEPTH
	highp vec3  eyeDir   = normalize(CameraPosition-position).xyz;
	highp float shinness = 4.0;
	shinness = normal.a + shinness;
	highp float attMax = 30.0;
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
		//gl_FragColor = vec4(depth,depth,depth,1.0);
		//gl_FragColor = vec4(position.xyz,1.0);
		//gl_FragColor = vec4(specularmap.xyz,1.0);
		Ambient = color * 0.2;
		Final+= Ambient;
		gl_FragColor = vec4(Final.xyz,1.0);
}
#else //G_BUFF_PASS
uniform highp sampler2D diffuse;
varying highp vec2 vecUVCoords;
void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	highp vec4 color = texture2D(diffuse,coords);
	//color = vec4(depth,depth,depth,1.0);
	color.w = 1.0;
	gl_FragColor = color;
}
#endif
