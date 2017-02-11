#include "ModelGL.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include "Timer.h"
#include <iostream>
void ModelGL::SetFileName(char * fileName)
{
	m_fileName = std::string(fileName);
}
void ModelGL::Create()
{
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS.glsl");
	char *fsSourceP = file2string("FS.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);

	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	normalAttribLoc = glGetAttribLocation(shaderID, "Normal");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");

	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");
	matWorldUniformLoc = glGetUniformLocation(shaderID, "World");




	Timer timer;
	timer.Init();
	if (!parser.LoadFile(m_fileName.c_str()))
	{
		std::cout << "Error: No se pudo cargar el modelo" << std::endl;
		return;
	}
		
	timer.Update();
	std::cout <<"Archivo cargado en: "<< timer.GetDTSecs() << " segundos..." << std::endl;



	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, parser.m_vertexSize * sizeof(vertexStruct), parser.m_vbo, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, parser.m_indexSize * 3 * sizeof( unsigned short), parser.m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	parser.Deallocate();

	transform = Identity();
}

void ModelGL::Transform(float * t)
{
	transform = t;
}

void ModelGL::Draw(float *t, float *vp)
{
	glUseProgram(shaderID);

	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(vp);
	Matrix4D WVP = transform*VP;

	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);


	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);

	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(16));

	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(32));

	glDrawElements(GL_TRIANGLES, parser.m_indexSize*3, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexAttribLoc);

	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}
	glUseProgram(0);
}

void ModelGL::Destroy()
{
}


ModelGL::~ModelGL()
{
}
