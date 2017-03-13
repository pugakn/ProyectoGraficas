uniform mediump sampler2D diffuse;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

varying highp vec3 vecTransformed;
#ifdef USE_PIXELLIGHTING
varying highp vec3 normalTransformed;
uniform highp vec3 lightDir;
#endif
#ifdef USE_VERTEXLIGHTING
varying highp float light_mod;
#endif
//

void main(){
#ifdef USE_TEXCOORD0
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb;
#else
	highp vec3 vector = normalize(vecTransformed*0.5 + 0.5);
#endif
	#ifdef USE_PIXELLIGHTING
	highp vec3 norm = normalize(normalTransformed);
	highp float light_mod = clamp(dot(normalTransformed,lightDir)/(length(normalTransformed)*length(lightDir)),0.0,1.0) ;
	#else
		#ifndef USE_VERTEXLIGHTING
			highp float light_mod = 1.0;
		#endif
	#endif
	gl_FragColor = vec4(vector* vec3(light_mod,light_mod,light_mod),1.0);
}
