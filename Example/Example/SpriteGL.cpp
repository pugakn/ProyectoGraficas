#include "SpriteGL.h"
#ifdef USING_OPENGL_ES
#include "TextureGL.h"
#include <iostream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


void SpriteGL::Create(Matrix4D &VP)
{
	m_VP = VP;
	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_Sprite.glsl");
	char *fsSourceP = file2string("Shaders/FS_Sprite.glsl");

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
	matWorldUniformLoc = glGetUniformLocation(shaderID, "WVP");

	m_VBO[0] = { 0.0f,  1.0f, 0.0f ,1.0f,  0.0f, 0.0f };//Left Top
	m_VBO[1] = { 0.0f,  0.0f, 0.0f ,1.0f,  0.0f, 1.0f };//Left Bot
	m_VBO[2] = { 1.0f,  0.0f, 0.0f ,1.0f,  1.0f, 1.0f };//Right Bot
	m_VBO[3] = { 1.0f,  1.0f, 0.0f ,1.0f,  1.0f, 0.0f };//Right Top

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(spriteVertex), &m_VBO[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indexBuffer[0] = 2;
	m_indexBuffer[1] = 1;
	m_indexBuffer[2] = 0;
	m_indexBuffer[3] = 3;
	m_indexBuffer[4] = 2;
	m_indexBuffer[5] = 0;

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vsSourceP;
	delete[] fsSourceP;

	transform = Identity();
	Scale = Identity();
	Position = Identity();
	//Cargar textura
	Texture *tex = new TextureGL;
	int textureID = tex->LoadTexture(const_cast<char*>(m_str));
	if (textureID != -1) {
		IdTex = textureID;
		IdTexUniformLoc = glGetUniformLocation(shaderID, "diffuse");
	}
	else {
		std::cout << "Texture not Found" << std::endl;
		delete tex;
	}
	textureWidth = tex->x;
}

void SpriteGL::Draw()
{
	glUseProgram(shaderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	Matrix4D WVP = transform*m_VP;
	//Set uniforms
	glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
	//Set Atributes
	glEnableVertexAttribArray(vertexAttribLoc);
	if (uvAttribLoc != -1)
		glEnableVertexAttribArray(uvAttribLoc);
	glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(spriteVertex), BUFFER_OFFSET(0));
	if (uvAttribLoc != -1)
		glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(spriteVertex), BUFFER_OFFSET(16));
	//Set texture
	glActiveTexture(GL_TEXTURE0);//Set Active texture unit
	glBindTexture(GL_TEXTURE_2D, IdTex);
	glUniform1i(IdTexUniformLoc, 0); //Specify location
									 //Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//Reset 
	if (uvAttribLoc != -1) {
		glDisableVertexAttribArray(uvAttribLoc);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexAttribLoc);
	glUseProgram(0);
}

void SpriteGL::Destroy()
{
	glDeleteProgram(shaderID);
}


SpriteGL::SpriteGL()
{
}


SpriteGL::~SpriteGL()
{
}

#endif