#include "GLFont.h"
#include "TextureGL.h"
#include <iostream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void GLFont::Create()
{
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("VS_Font.glsl");
	char *fsSourceP = file2string("FS_Font.glsl");

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, vsSourceP);
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(shaderID, vshader_id);
	glAttachShader(shaderID, fshader_id);

	glLinkProgram(shaderID);
	glUseProgram(shaderID);
	//Attributes
	vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
	uvAttribLoc = glGetAttribLocation(shaderID, "UV");
	//Uniforms
	matWorldViewProjUniformLoc = glGetUniformLocation(shaderID, "WVP");

	// -Z SIDE
	m_VBO[0] = { -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f };
	m_VBO[1] = { 1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f };
	m_VBO[2] = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	m_VBO[3] = { 1.0f, -1.0f, -1.0f, 1.0f,  0.0f, 1.0f };


	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(fontVertex), &m_VBO[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// -Z
	m_indexBuffer[0] = 20;
	m_indexBuffer[1] = 22;
	m_indexBuffer[2] = 21;
	m_indexBuffer[3] = 21;
	m_indexBuffer[4] = 22;
	m_indexBuffer[5] = 23;

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vsSourceP;
	delete[] fsSourceP;

	transform = Identity();

	//Cargar textura de fuente
	Texture *tex = new TextureGL;
	int textureID = tex->LoadTexture(const_cast<char*>("Fonts/ArialFont_0.tga"));
	if (textureID != -1) {
		IdTex = textureID;
		IdTexUniformLoc = glGetUniformLocation(shaderID, "diffuse");
	}
	else {
		std::cout << "Texture not Found" << std::endl;
		delete tex;
	}
	//Cargar propiedades de fuente
	font.LoadFile("Fonts/ArialFont.fnt");

}

void GLFont::Transform(float * t)
{
	transform = t;
}

void GLFont::Draw(float * t)
{
	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D WVP = transform*VP;

	glUseProgram(shaderID);
	//glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
	glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glEnableVertexAttribArray(vertexAttribLoc);

	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);

	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(fontVertex), BUFFER_OFFSET(0));

	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(fontVertex), BUFFER_OFFSET(32));
	//Draw
	glDrawElements(GL_TRIANGLES, 2 , GL_UNSIGNED_SHORT, 0);

	//Reset 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexAttribLoc);

	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}
	glUseProgram(0);


}

void GLFont::Destroy()
{
	glDeleteProgram(shaderID);
}

GLFont::GLFont()
{
}


GLFont::~GLFont()
{
}
