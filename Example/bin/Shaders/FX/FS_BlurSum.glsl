uniform highp sampler2D texture01;
uniform highp sampler2D texture02;
varying highp vec2 vecUVCoords;
highp float lerp(highp vec4 a, highp vec4 b, highp float f)
{
    return a + f * (b - a);
}
void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	highp vec4 color = texture2D(texture01,coords);
	highp vec4 color2 = texture2D(texture02,coords);
	highp vec4 color3 = mix(color,color2,0.36);
	//highp vec4 color3 = color + color2;
	color3.a = 1.0;
	gl_FragColor = color3;
}
