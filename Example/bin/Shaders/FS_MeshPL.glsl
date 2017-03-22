uniform mediump sampler2D diffuse;
uniform highp vec3 lightColor;
#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

varying highp vec3 pixelPos;
#ifdef USE_PIXELLIGHTING
varying highp vec3 normalTransformed;
uniform highp vec3 light;
#endif
#ifdef USE_VERTEXLIGHTING
varying highp float light_mod;
#endif
#ifdef USE_SPECULAR
uniform highp vec3 camPos;
#endif
//

void main(){
#ifdef USE_TEXCOORD0
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb;
#else
	highp vec3 vector = normalize(pixelPos*0.5 + 0.5);
#endif
	#ifdef USE_PIXELLIGHTING
	highp vec3 norm = normalize(normalTransformed);
	highp vec3 lightDir = normalize(light - pixelPos);
	highp float lightDist = length(pixelPos - light);
	highp float light_mod = clamp(dot(norm,lightDir),0.0,1.0) ;
	#ifdef USING_ATENUATION
	light_mod = min(light_mod / ((lightDist * lightDist)/12000.0),light_mod );
	#endif
	#ifdef USE_SPECULAR
		highp vec3 specularCol = lightColor;
		highp vec3 RL = reflect(lightDir,norm);
		highp vec3 eyeDir = normalize(camPos - pixelPos);
		highp float specular = pow(dot(-eyeDir,RL),50.0);
	#endif
	#else
		#ifndef USE_VERTEXLIGHTING
			highp float light_mod = 1.0;
		#endif
	#endif

	#ifdef USE_SPECULAR
	gl_FragColor = vec4(vector*0.2,1.0) + vec4(vector* vec3(light_mod,light_mod,light_mod) * lightColor,1.0) + vec4(specularCol,1.0)* specular  ;
	#else
	gl_FragColor = vec4(vector*0.2,1.0) + vec4(vector* vec3(light_mod,light_mod,light_mod) * lightColor,1.0);
	#endif
}
