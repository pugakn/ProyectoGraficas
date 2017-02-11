#pragma once
#include "PrimitiveBase.h"
#include "MeshParser.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Matrix4D.h"

class ModelGL : public PrimitiveBase
{
private:
	GLuint	shaderID;
	GLuint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	Matrix4D	transform;

	GLuint			VB;
	GLuint			IB;

	std::string m_fileName;
public:
	void SetFileName(char* fileName);
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t, float *vp) override;
	void Destroy() override;
	MeshParser parser;
	ModelGL() : shaderID(0) {};
	~ModelGL();
};

