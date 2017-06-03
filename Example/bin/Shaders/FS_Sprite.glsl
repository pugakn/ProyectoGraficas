varying highp vec2 vecUVCoords;
uniform mediump sampler2D diffuse;
void main(){
highp vec4 color = texture2D(diffuse,vecUVCoords);
gl_FragColor = color;
}
