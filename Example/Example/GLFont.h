#pragma once
#include "Config.h"
#ifdef USING_OPENGL_ES
#include "TextMesh.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
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
