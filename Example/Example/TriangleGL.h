#ifndef UAD_TRIANGLEGL_H
#define UAD_TRIANGLEGL_H
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


//#define USE_ARRAY_OF_STRUCTS
#define USE_VBO

#ifdef USE_VBO
struct triVertex {
	float x, y, z;
	float r, g, b;
};
#else
	#ifdef USE_ARRAY_OF_STRUCTS
	struct triVertex{
		float x,y,z;
		float r,g,b;
	};
	#else
	struct triVertex {
		float x, y, z;
	};
	#endif
#endif

class TrangleGL : public PrimitiveBase {
public:
	TrangleGL() : shaderID(0) {}
	void Create();
	void Transform(float *t);
	void Draw(float *t);
	void Destroy();
	void SetShaderBySignature(unsigned long sig) override;
	GLuint	shaderID;
	GLuint	vertexAttribLoc;
	GLuint	colorAttribLoc;
	
	GLuint  matUniformLoc;
#ifdef USE_VBO
	triVertex		vertices[4];
	unsigned short	indices[6];
	GLuint			VB;
	GLuint			IB;
#else
	#ifdef USE_ARRAY_OF_STRUCTS
		triVertex	vertices[6];
	#else
		triVertex	positions[6];
		triVertex	colors[6];
	#endif
#endif
	D3DXMATRIX	transform;
};

#endif//USING_OPENGL_ES
#endif 
