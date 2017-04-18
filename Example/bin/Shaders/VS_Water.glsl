attribute highp vec4 Vertex;
varying highp vec4 MVPVertex;

uniform highp mat4 WVP;

void main(){
	MVPVertex = WVP*Vertex;
	gl_Position = MVPVertex;
}
