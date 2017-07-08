attribute highp vec4 Vertex;
attribute highp vec2 UV;

uniform highp mat4 World;
varying highp vec2 vecUVCoords;

void main(){
	vecUVCoords = UV;
	gl_Position = World*Vertex;
}

