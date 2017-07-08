uniform highp sampler2D texture01;
uniform highp vec2 ShadowTexSize;
varying highp vec2 vecUVCoords;
void main(){
	//highp float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	highp vec4 color = texture2D(texture01,coords);
	highp float pixelOffset = 1.0/ShadowTexSize.x;

	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 8, 0.0)) * 0.004216;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 7, 0.0)) * 0.006216;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 6, 0.0)) * 0.010216;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 5, 0.0)) * 0.016216;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 4, 0.0)) * 0.054054;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 3, 0.0)) * 0.1216216;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 2, 0.0)) * 0.1945946;
	color += texture2D(texture01, coords - lowp vec2(pixelOffset * 1, 0.0)) * 0.227027;
	
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 1, 0.0)) * 0.227027;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 2, 0.0)) * 0.1945946;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 3, 0.0)) * 0.1216216;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 4, 0.0)) * 0.054054;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 5, 0.0)) * 0.016216;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 6, 0.0)) * 0.010216;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 7, 0.0)) * 0.006216;
	color += texture2D(texture01, coords + lowp vec2(pixelOffset * 8, 0.0)) * 0.004216;
	color.a = 1.0;
	gl_FragColor = color;
}
