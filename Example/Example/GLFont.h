#pragma once
#include "Config.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "UtilsGL.h"
#include "Font.h"
#include "PrimitiveBase.h"
#include <string>
class GLFont 
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
	GLint matWorldUniformLoc;
	GLint xOffsetLoc;
	GLint yOffsetLoc;
	GLint xSeparationLoc;
	GLint ySeparationLoc;

	Matrix4D transform;
	Font font;

	fontVertex m_VBO[4];
	unsigned short m_indexBuffer[6];
	GLuint	IB;
	GLuint	VB;

	int IdTex;
	GLint IdTexUniformLoc;

	float textureWidth;


	//================
	Matrix4D		Position;
	Matrix4D		Scale;
public:
	std::string m_text;
	void Update();
	void TranslateAbsolute(float x, float y, float z);

	void Create() ;
	void Transform(float *t) ;
	void Draw() ;
	void Destroy() ;
	GLFont();
	~GLFont();
};

