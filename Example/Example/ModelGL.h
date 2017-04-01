#pragma once
#include "Config.h"
#ifdef USING_OPENGL_ES
#include "PrimitiveBase.h"
#include "MeshParser.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Matrix4D.h"
#include "Wireframe.h"

class Texture;
class ModelGL : public PrimitiveBase
{
private:
	//GLuint	shaderID;
	struct TextureInfo
	{
		std::vector <int> IdsTex;
		std::vector <GLint> IdTexUniformLocs;
	};
	struct SubsetInfo
	{
		GLuint shadersID;
		//Atributes
		GLint	 vertexAttribLocs;
		GLint	 normalAttribLocs;
		GLint	 binormalAttribLocs;
		GLint	 tangentAttribLocs;
		GLint	 uvAttribLocs;
		//Uniforms
		GLint  matWorldViewProjUniformLoc;
		GLint  matWorldUniformLoc;

		GLint lightLoc;
		GLint lightColLoc;
		GLint camPosLoc;
		GLint specExpLoc;
		GLint attMaxLoc;

		//Index Bufer ID
		GLuint	IB;
		//Textures
		TextureInfo textInfo;

	};
	struct MeshInfo
	{
		std::vector<SubsetInfo> subsetInfo;
	};

	struct WireframeInfo
	{
		GLuint IB;
		GLuint ShaderID;
		GLint MatWorldViewProjUniformLoc;
		GLint VertexAttribLoc;
	};
	
	std::vector<MeshInfo> m_meshInfo;
	Vector3D lightColor;
	Matrix4D transform;
	GLuint	 VB;
	std::string m_fileName;


	std::vector<Texture*> Textures;
	Wireframe wireframe;
	WireframeInfo wire;


	
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

