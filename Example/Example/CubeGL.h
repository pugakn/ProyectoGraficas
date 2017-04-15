#ifndef UAD_CUBE_GL_H
#define UAD_CUBE_GL_H
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


#include <d3dx9math.h>
#include "PrimitiveBase.h"
#include "UtilsGL.h"




struct CVertex {

	float x, y, z,w;
	float nx,ny,nz,nw;
	float s,t;
};

class CubeGL : public PrimitiveBase {
public:
	CubeGL() : shaderID(0) {}
	void Create();
	void Transform(float *t);
	void Draw(float *t);
	void Destroy();

	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	CVertex			vertices[24];
	unsigned short	indices[36];
	GLuint			VB;
	GLuint			IB;

	Matrix4D	transform;
};

#endif // USING_OPENGL_ES
#endif