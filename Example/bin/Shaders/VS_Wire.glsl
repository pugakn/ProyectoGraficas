attribute highp vec4 Vertex;

uniform highp mat4 WVP;

void main(){
	gl_Position = WVP*Vertex;
}
