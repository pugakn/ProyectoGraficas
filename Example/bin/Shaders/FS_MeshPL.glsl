uniform mediump sampler2D diffuse;
uniform highp vec3 lightColor;
#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

varying highp vec3 pixelPos;
#ifdef USE_PIXELLIGHTING
uniform highp vec3 light;
#endif
#ifdef USE_VERTEXLIGHTING
varying highp float light_mod;
#endif
#if defined USE_SPECULAR_BLIN || defined USE_SPECULAR_PHONG
uniform highp vec3 camPos;
uniform highp float specExp;
#endif
#ifdef USING_ATENUATION
uniform highp float attMax;
#endif
#ifdef USE_SPEC_MAP
uniform mediump sampler2D specularMap;
#endif
#ifdef USE_GLOSS_MAP
uniform mediump sampler2D glossMap;
#endif
#ifdef USE_NORMAL_MAP
uniform mediump sampler2D normalMap;
#endif
#ifdef USE_TANGENTS
varying highp vec3 tangentTransformed;
#endif
#ifdef USE_NORMALS
varying highp vec3 normalTransformed;
#endif
#ifdef USE_BINORMALS
varying highp vec3 binormalTransformed;
#endif
//

void main(){
	//float clipPos = dot (pixelPos, vec3(0,1,0)) + w;
	  //  if (clipPos < 0.0) {
	   //     discard;
	  //  }


	lowp vec4 color = vec4(0.0,0.0,0.0,1.0);
#ifdef USE_TEXCOORD0
		#ifdef USE_TANGENTS
		lowp vec3 tang = normalize(tangentTransformed);
		#endif
		#ifdef USE_BINORMALS
		lowp vec3 binor = normalize(binormalTransformed);
		#endif
	#ifdef USE_NORMAL_MAP
		lowp vec3 norm = texture2D(normalMap,vecUVCoords).xyz * 2.0 - 1.0;
		norm.g = -norm.g;
		highp mat3 TBN = mat3(tang,binor,normalize(normalTransformed));
		norm = TBN * norm;
		norm = normalize(norm);
	#else
		lowp vec3 norm = normalize(normalTransformed);
	#endif
		color = texture2D(diffuse,vecUVCoords);
#else
	color = normalize(pixelPos*0.5 + 0.5);
#endif
#ifdef USE_PIXELLIGHTING
	lowp vec3 lightDir = normalize(light - pixelPos);
//	highp float light_mod = clamp(dot(norm,lightDir),0.0,1.0) ;
	highp float att = pow(dot(norm,lightDir) * 0.5 + 0.5,2.0);
	highp float light_mod = clamp( att,0.0,1.0) ;
	#ifdef USE_SPECULAR_PHONG
		lowp vec3 RL = reflect(lightDir,norm);
		lowp vec3 eyeDir = normalize(camPos - pixelPos);
		highp float specular = pow(dot(-eyeDir,RL),specExp);
	#elif defined USE_SPECULAR_BLIN
		lowp vec3 eyeDir = normalize(camPos - pixelPos);
		lowp vec3 RL = normalize(eyeDir+lightDir);
		highp float specular = dot(RL,norm) * 0.5 + 0.5;
		#ifdef USE_GLOSS_MAP
					specular = pow( specular ,texture2D(glossMap,vecUVCoords).r);
					//specular = pow( specular ,specExp);
					#else
					specular = pow( specular ,specExp);
		#endif
  #endif
	#ifdef USING_ATENUATION
		highp float lightDist = length(pixelPos - light);
		light_mod = min(light_mod / ((lightDist * lightDist)/attMax),light_mod );
		specular = min(specular / ((lightDist * lightDist)/attMax),specular);
	#endif
#else
	#ifndef USE_VERTEXLIGHTING
		highp float light_mod = 1.0;
	#endif
#endif
//Ambient
lowp vec4 Ambient = color * vec4(0.3,0.3,0.3,1);
//Lambert
lowp vec4 Lambert = vec4(lightColor,1.0);
Lambert*= color * vec4(light_mod,light_mod,light_mod,1.0);
//Specular
lowp vec4 Specular = vec4(lightColor,1.0);
Specular *= specular;
#ifdef USE_SPEC_MAP
	Specular *= texture2D(specularMap,vecUVCoords);
#endif
Specular *= att;
#if defined USE_SPECULAR_BLIN || defined USE_SPECULAR_PHONG
gl_FragColor = Ambient + Lambert + Specular;
#else
gl_FragColor = Ambient + Lambert;
#endif

}
