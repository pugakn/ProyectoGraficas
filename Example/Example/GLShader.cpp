#include "GLShader.h"



void GLShader::LoadAPI()
{
	vshader_id = createShader(GL_VERTEX_SHADER, const_cast<char*>(vstr.c_str()));
	fshader_id = createShader(GL_FRAGMENT_SHADER, const_cast<char*>(fstr.c_str()));
	if (vshader_id == 0 || fshader_id == 0)
	{
		ShaderID = Tools::DefaultShaderID;
	}
	else 
	{
		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, vshader_id);
		glAttachShader(ShaderID, fshader_id);
	}
}

GLShader::GLShader()
{
}


GLShader::~GLShader()
{
}
