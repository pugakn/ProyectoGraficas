#pragma once
#include "BaseShader.h"
#ifdef USING_GL_COMMON
#ifdef USING_OPENGL_ES20
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3\gl31.h>
#elif defined(USING_OPENGL)
#include <GL\glew.h>
#endif
class GLShader : public Shader
{

public:
	struct
	{
		GLint	 vertexAttribLocs;
		GLint	 normalAttribLocs;
		GLint	 binormalAttribLocs;
		GLint	 tangentAttribLocs;
		GLint	 uvAttribLocs;
		//Uniforms
		GLint  matWorldViewProjUniformLoc;
		GLint  matWorldViewUniformLoc;
		GLint  matWorldUniformLoc;

		GLint lightLoc;
		GLint lightColLoc;
		GLint camPosLoc;
		GLint specExpLoc;
		GLint attMaxLoc;
		GLint camFarLoc;
	} m_locs;
	GLuint vshader_id;
	GLuint fshader_id;
	GLint ShaderID;
	void LoadAPI()override;
	GLShader();
	~GLShader();
};
#endif

