uniform highp sampler2D diffuse;
varying highp vec4 MVPVertex;
void main(){
//Coordenadas proyectivas
	lowp vec2 projCoord = vec2(1.0 / MVPVertex.w);
	projCoord = MVPVertex.xy * projCoord;
	projCoord += vec2(1.0);
	projCoord *= vec2(0.5);
	projCoord = clamp(projCoord, 0.001, 0.999);



	lowp vec2 proj = MVPVertex.xy / MVPVertex.w;
  lowp vec2 TexCoord = 0.5 * (proj + 1.0);
	lowp vec4 reflectionColor = mix(texture2D(diffuse, projCoord), vec4(0.1,0.2,0.6,1.0), 0.2);


		//highp vec4 reflectionColor = texture2D(diffuse, projCoord.xy);

	gl_FragColor = reflectionColor;
}
