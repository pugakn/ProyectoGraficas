#pragma once
#include "Config.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Font.h"
#include "PrimitiveBase.h"
class GLFont : public PrimitiveBase
{
	struct fontVertex
	{
		float x, y, z, w;
		float u, v;
	};
private:
	GLuint shaderID;
	GLint vertexAttribLoc;
	GLint uvAttribLoc;
	GLint matWorldViewProjUniformLoc;

	Matrix4D transform;
	Font font;

	fontVertex m_VBO[4];
	unsigned int m_indexBuffer[6];
	GLuint	IB;
	GLuint	VB;

	int IdTex;
	GLint IdTexUniformLoc;
public:
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t) override;
	void Destroy() override;
	GLFont();
	~GLFont();
};

