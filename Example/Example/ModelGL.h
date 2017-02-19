#pragma once
#include "PrimitiveBase.h"
#include "MeshParser.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Matrix4D.h"

#include "Texture.h"
#include "TextureGL.h"

class ModelGL : public PrimitiveBase
{
private:
	//GLuint	shaderID;
	std::vector <GLuint>	shadersID;
	std::vector <GLuint>	vertexAttribLocs;
	std::vector <GLuint>	normalAttribLocs;
	std::vector <GLuint>	uvAttribLocs;
	std::vector <GLuint> IdTexUniformLocs;
	std::vector <int> IdsTex;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	Matrix4D	transform;

	GLuint			VB;
	std::vector <GLuint>			IBs;

	std::string m_fileName;
	std::vector<Texture*> Textures;
public:
	void SetFileName(char* fileName);
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t, float *vp) override;
	void Destroy() override;
	MeshParser parser;
	ModelGL()  {};
	~ModelGL();
};

