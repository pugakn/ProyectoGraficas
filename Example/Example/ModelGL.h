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

#include "PrimitiveBase.h"
#include "MeshParser.h"
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
		SubsetInfo() {
			vertexAttribLocs = -1;
			normalAttribLocs = -1;
			binormalAttribLocs = -1;
			tangentAttribLocs = -1;
			uvAttribLocs = -1;
			matWorldViewProjUniformLoc = -1;
			matWorldUniformLoc = -1;

			lightLoc = -1;
			lightColLoc = -1;
			camPosLoc = -1;
			specExpLoc = -1;
			attMaxLoc = -1;
		}

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


	inline void DrawMeshes(const Matrix4D &VP, const Matrix4D &WVP);
	inline void DrawWireframe(const Matrix4D &VP, const Matrix4D &WVP);
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

