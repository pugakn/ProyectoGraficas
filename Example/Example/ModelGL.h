#pragma once
#include "Config.h"
#ifdef USING_OPENGL_ES
#include "PrimitiveBase.h"
#include "MeshParser.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Matrix4D.h"

class Texture;
struct TextureInfo
{
	std::vector <int> IdsTex;
	std::vector <GLint> IdTexUniformLocs;
};
class ModelGL : public PrimitiveBase
{
private:
	//GLuint	shaderID;
	std::vector <GLuint>	shadersID;
	std::vector <GLint>	vertexAttribLocs;
	std::vector <GLint>	normalAttribLocs;
	std::vector <GLint>	binormalAttribLocs;
	std::vector <GLint>	tangentAttribLocs;
	std::vector <GLint>	uvAttribLocs;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;
	GLint lightLoc;
	GLint lightColLoc;
	GLint camPosLoc;
	GLint specExpLoc;
	GLint attMaxLoc;

	Vector3D lightColor;

	Matrix4D	transform;

	GLuint					VB;
	std::vector <GLuint>	IBs;

	std::string m_fileName;


	std::vector<TextureInfo> textureInfo;
	std::vector<Texture*> Textures;
	std::vector<Texture*> SpecularTextures;
public:
	void SetFileName(char* fileName);
	void Create() override;
	void Transform(float *t) override;
	void Draw(float *t) override;
	void Destroy() override;
	MeshParser parser;
	ModelGL()  {};
	~ModelGL();
};
#endif

