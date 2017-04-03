attribute highp vec4 Vertex;
attribute highp vec2 UV;

uniform highp mat4 World;
uniform highp float xSeparation;
uniform highp float ySeparation;

varying highp vec2 vecUVCoords;

void main(){
	vecUVCoords = UV;
	gl_Position = World*Vertex;
	gl_Position.x += xSeparation;
	gl_Position.y -= ySeparation;
}
