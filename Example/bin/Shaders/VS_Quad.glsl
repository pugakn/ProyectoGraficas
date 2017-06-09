#ifdef G_BUFF_PASS
//uniform highp mat4 WVP;
//uniform highp mat4 World;
//uniform highp mat4 WorldView;
uniform highp mat4 VPInverse;
//uniform highp vec4 LightPositions[128];
//uniform highp vec4 LightColors[128];
uniform highp vec4 CameraPosition;
//uniform highp vec4 CameraInfo;


uniform highp mat4 W;
varying highp vec2 vecUVCoords;
varying highp vec4 PosCorner;

attribute highp vec4 Vertex;
attribute highp vec2 UV;
void main(){
	vecUVCoords = UV;
	PosCorner = VPInverse*vec4(Vertex.xy,1.0,1.0);
	PosCorner.xyz /= PosCorner.w;
	PosCorner = PosCorner - CameraPosition;
	gl_Position = W*Vertex;
}

#else

attribute highp vec4 Vertex;
attribute highp vec2 UV;

uniform highp mat4 W;
varying highp vec2 vecUVCoords;

void main(){
	vecUVCoords = UV;
	gl_Position = W*Vertex;
}
#endif
