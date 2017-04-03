varying highp vec2 vecUVCoords;
uniform mediump sampler2D diffuse;

uniform highp float xOffset;
uniform highp float yOffset;


void main(){
highp vec2  vTexCoord=vec2(vecUVCoords.x + xOffset,vecUVCoords.y+ yOffset);
highp float alpha = texture2D(diffuse,vTexCoord).a;
lowp vec3 color = vec3(1.0,0.0,0.0);
gl_FragColor = vec4(color,alpha);
}
