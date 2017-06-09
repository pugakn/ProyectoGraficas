#include "GLShader.h"

#ifdef USING_GL_COMMON

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
		glLinkProgram(ShaderID);


		//Obtener locaciones de Attributes
		m_locs.vertexAttribLocs = glGetAttribLocation(ShaderID, "Vertex");
		m_locs.normalAttribLocs = glGetAttribLocation(ShaderID, "Normal");
		m_locs.binormalAttribLocs = glGetAttribLocation(ShaderID, "Binormal");
		m_locs.tangentAttribLocs = glGetAttribLocation(ShaderID, "Tangent");
		m_locs.uvAttribLocs = glGetAttribLocation(ShaderID, "UV");

		//Obtener locaciones de Uniforms
		m_locs.matWorldViewUniformLoc = glGetUniformLocation(ShaderID, "WV");
		m_locs.matWorldViewProjUniformLoc = glGetUniformLocation(ShaderID, "WVP");
		m_locs.matWorldUniformLoc = glGetUniformLocation(ShaderID, "World");
		m_locs.lightLoc = glGetUniformLocation(ShaderID, "light");
		m_locs.lightColLoc = glGetUniformLocation(ShaderID, "lightColor");
		m_locs.camPosLoc = glGetUniformLocation(ShaderID, "camPos");
		m_locs.specExpLoc = glGetUniformLocation(ShaderID, "specExp");
		m_locs.attMaxLoc = glGetUniformLocation(ShaderID, "attMax");
		m_locs.camFarLoc = glGetUniformLocation(ShaderID, "camFar");
	}
}

GLShader::GLShader()
{
}


GLShader::~GLShader()
{
}
#endif
