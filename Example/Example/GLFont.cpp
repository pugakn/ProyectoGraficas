#include "GLFont.h"
#ifdef USING_GL_COMMON
#include "TextureGL.h"
#include <iostream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void GLFont::Create()
{
	vertexAttribLoc = -1;
	uvAttribLoc = -1;
	matWorldViewProjUniformLoc = -1;
	matWorldUniformLoc = -1;
	xOffsetLoc = -1;
	yOffsetLoc = -1;
	xSeparationLoc = -1;
	ySeparationLoc = -1;
	IdTexUniformLoc = -1;

	shaderID = glCreateProgram();

	char *vsSourceP = file2string("Shaders/VS_Font.glsl");
	char *fsSourceP = file2string("Shaders/FS_Font.glsl");
	std::string vsrc;
	std::string fsrc;
	if (vsSourceP && fsSourceP)
	{
		vsrc = std::string(vsSourceP);
		fsrc = std::string(fsSourceP);
	}
	std::string Defines;
#ifdef USING_OPENGL
	if (vsSourceP != NULL && fsSourceP != NULL)
	{
		Defines += "#define lowp\n\n";
		Defines += "#define mediump\n\n";
		Defines += "#define highp\n\n";
	}
#endif // USING_OPENGL
	vsrc = Defines + vsrc;
	fsrc = Defines + fsrc;

	GLuint vshader_id = createShader(GL_VERTEX_SHADER, (char*)vsrc.c_str());
	GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, (char*)fsrc.c_str());
	if (vshader_id == 0 || fshader_id == 0)
	{
		shaderID = Tools::DefaultShaderID;
		glLinkProgram(shaderID);
		glUseProgram(shaderID);

		vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
		matWorldUniformLoc = glGetUniformLocation(shaderID, "WVP");
	}
	else
	{
		glAttachShader(shaderID, vshader_id);
		glAttachShader(shaderID, fshader_id);

		glLinkProgram(shaderID);
		glUseProgram(shaderID);
		//Attributes
		vertexAttribLoc = glGetAttribLocation(shaderID, "Vertex");
		uvAttribLoc = glGetAttribLocation(shaderID, "UV");
		//Uniforms
		matWorldUniformLoc = glGetUniformLocation(shaderID, "World");
		xOffsetLoc = glGetUniformLocation(shaderID, "xOffset");
		yOffsetLoc = glGetUniformLocation(shaderID, "yOffset");
		xSeparationLoc = glGetUniformLocation(shaderID, "xSeparation");
		ySeparationLoc = glGetUniformLocation(shaderID, "ySeparation");



		//Cargar textura de fuente
		IdTex = Tools::LoadTexture("ArialFont_0.tga");
		Texture *tex = Tools::GetTexture(IdTex);
		IdTexUniformLoc = glGetUniformLocation(shaderID, "diffuse");
		textureWidth = tex->x;
	}
	//Cargar propiedades de fuente
	font.LoadFile("Fonts/ArialFont.fnt");
	m_VBO[0] = { 0.0f,  1.0f, 0.0f ,1.0f,  0.0f, 0.0f };//Left Top
	m_VBO[1] = { 0.0f,  0.0f, 0.0f ,1.0f,  0.0f, 1.0f };//Left Bot
	m_VBO[2] = { 1.0f,  0.0f, 0.0f ,1.0f,  1.0f, 1.0f };//Right Bot
	m_VBO[3] = { 1.0f,  1.0f, 0.0f ,1.0f,  1.0f, 0.0f };//Right Top

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(fontVertex), &m_VBO[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indexBuffer[0] = 2;
	m_indexBuffer[1] = 1;
	m_indexBuffer[2] = 0;
	m_indexBuffer[3] = 3;
	m_indexBuffer[4] = 2;
	m_indexBuffer[5] = 0;

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6* sizeof(unsigned short), m_indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vsSourceP;
	delete[] fsSourceP;

	transform = Identity();
	Scale = Identity();
	Position = Identity();
	m_text = "Inserte Texto 'XD";
}
void GLFont::Draw()
{
	glUseProgram(shaderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	float xSeparation = 0;
	for (size_t i = 0; i < m_text.size(); i++)
	{
		
		int x = m_text[i] - 32;
		//============= Update VBO =================
		float height = font.m_charInfo[x].height / textureWidth;
		float width = font.m_charInfo[x].width / textureWidth;
		fontVertex newVBO[4];
		newVBO[0] = { 0.0f, height, 0.0f, 1.0f, 0.0f,   0.0f };//Left Top
		newVBO[1] = { 0.0f,0.0f,0.0f ,1.0f,0.0f, height };//Left Bot
		newVBO[2] = { width ,  0.0f,  0.0f ,1.0f,	width, height };//Right Bot
		newVBO[3] = { width,  height, 0.0f ,1.0f,   width, 0.0f };//Right Top

		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(fontVertex), &newVBO[0], GL_STATIC_DRAW);
		//Set uniforms
		xSeparation += font.m_charInfo[x].xoffset / textureWidth;
		float ySeparation = 0;//TODO: Agregar separación en y
		if (matWorldUniformLoc != -1)
			glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
		if (xOffsetLoc != -1)
			glUniform1f(xOffsetLoc, font.m_charInfo[x].x / textureWidth);
		if (yOffsetLoc != -1)
			glUniform1f(yOffsetLoc, font.m_charInfo[x].y / textureWidth);
		if (xSeparationLoc != -1)
			glUniform1f(xSeparationLoc, xSeparation);
		if (ySeparationLoc != -1)
			glUniform1f(ySeparationLoc, ySeparation);
		//Set Atributes
		glEnableVertexAttribArray(vertexAttribLoc);
		if (uvAttribLoc != -1)
			glEnableVertexAttribArray(uvAttribLoc);
		glVertexAttribPointer(vertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(fontVertex), BUFFER_OFFSET(0));
		if (uvAttribLoc != -1)
			glVertexAttribPointer(uvAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(fontVertex), BUFFER_OFFSET(16));
		//Set texture
		if (IdTexUniformLoc != -1)
		{
			glActiveTexture(GL_TEXTURE0);//Set Active texture unit
			glBindTexture(GL_TEXTURE_2D, IdTex);
			glUniform1i(IdTexUniformLoc, 0); //Specify location
		}
		//Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		xSeparation += font.m_charInfo[x].xadvance/textureWidth;
		//Reset 
		if (uvAttribLoc != -1) {
			glDisableVertexAttribArray(uvAttribLoc);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexAttribLoc);
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
#endif