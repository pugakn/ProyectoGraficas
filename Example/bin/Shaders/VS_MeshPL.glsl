attribute highp vec4 Vertex;


#ifdef USE_NORMALS
attribute highp vec4 Normal;
#endif

#ifdef USE_TANGENTS
attribute highp vec4 Tangent;
#endif

#ifdef USE_BINORMALS
attribute highp vec4 Binormal;
#endif

#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
#endif

varying highp vec3 pixelPos;

#ifdef USE_NORMALS
varying highp vec3 normalTransformed;
#endif
#ifdef USE_TANGENTS
varying highp vec3 tangentTransformed;
#endif
#ifdef USE_BINORMALS
varying highp vec3 binormalTransformed;
#endif
#ifdef USE_VERTEXLIGHTING
varying highp float light_mod;
uniform highp vec3 light;
#endif



#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

uniform highp mat4 WVP;
uniform highp mat4 World;
#ifdef LINEAR_DEPTH
uniform highp mat4 WV;
#endif

varying highp vec4 pos;
void main(){
	highp mat3 m3World = mat3(World);
	pixelPos	= (World*Vertex).xyz;
	#ifndef LINEAR_DEPTH
	pos = WVP * Vertex;
	#else
  pos = WV * Vertex;
	#endif

#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
#endif
#ifndef LINEAR_DEPTH
	gl_Position = pos;
#else
	gl_Position = WVP*Vertex;
#endif
#ifdef USE_NORMALS
	normalTransformed = normalize(m3World*Normal.rgb);
#endif
#ifdef USE_TANGENTS
	tangentTransformed = normalize(m3World*Tangent.rgb);
#endif
#ifdef USE_BINORMALS
	binormalTransformed = normalize(m3World*Binormal.rgb);
#endif
#ifdef USE_VERTEXLIGHTING
	highp vec3 norm = normalize(m3World*Normal.rgb);
	highp vec3 lightDir = normalize(light - (World*Vertex).xyz);
	highp float lightDist = length((World*Vertex).xyz - light);
	light_mod = clamp(dot(norm,lightDir),0.0,1.0);
	#ifdef USING_ATENUATION
	light_mod = min(light_mod / ((lightDist * lightDist)/15000.0),light_mod );
	#endif
#endif
}
