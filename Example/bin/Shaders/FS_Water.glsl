uniform highp sampler2D diffuse;
varying highp vec4 MVPVertex;
void main(){
//Coordenadas proyectivas



	lowp vec2 proj = MVPVertex.xy / MVPVertex.w;
  lowp vec2 TexCoord = 0.5 * (proj + 1.0);
	lowp vec4 reflectionColor = mix(texture2D(diffuse, TexCoord), vec4(0.1,0.2,0.6,1.0), 0.2);


		//highp vec4 reflectionColor = texture2D(diffuse, projCoord.xy);

	gl_FragColor = reflectionColor;
}
