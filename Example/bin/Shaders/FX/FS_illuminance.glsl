uniform highp sampler2D texture01;
varying highp vec2 vecUVCoords;
void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	highp vec4 color = texture2D(texture01,coords);
	color = color * color * color * color;
	color.w = 1.0;
	gl_FragColor = color;
}
