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
	light_mod = min(light_mod / ((lightDist * lightDist)/15000.0),light_mod );
	#endif
	#else
		#ifndef USE_VERTEXLIGHTING
			highp float light_mod = 1.0;
		#endif
	#endif
	gl_FragColor = vec4(vector*0.3,1.0) + vec4(vector* vec3(light_mod,light_mod,light_mod) * lightColor,1.0);
}
