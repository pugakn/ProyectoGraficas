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
#include "GLShader.h"

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
		//Index Bufer ID
		GLuint	IB;
		//Textures
		TextureInfo textInfo;
		std::vector<Shader*> m_shaderSet;

		unsigned long sig;
		SubsetInfo() {
			sig = 0;
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
	void SetShaderType(Shader::TYPE type) override;
	MeshParser parser;
	ModelGL() { useLight = true; };
	~ModelGL();
};
#endif

