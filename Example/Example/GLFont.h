#pragma once
#include "Config.h"
#ifdef USING_GL_COMMON
#ifdef USING_OPENGL_ES20
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3\gl31.h>
#elif defined(USING_OPENGL)
#include <GL\glew.h>
#endif
#include "TextMesh.h"
class GLFont : public TextMesh
{
private:
	GLuint shaderID;
	GLint vertexAttribLoc;
	GLint uvAttribLoc;
	GLint matWorldViewProjUniformLoc;
	GLint matWorldUniformLoc;
	GLint xOffsetLoc;
	GLint yOffsetLoc;
	GLint xSeparationLoc;
	GLint ySeparationLoc;


	GLuint	IB;
	GLuint	VB;

	int IdTex;
	GLint IdTexUniformLoc;


public:
	void Create() override;
	void Draw() override;
	void Destroy() override;
	GLFont();
	~GLFont();
};
#endif
