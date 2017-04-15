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
#include "Sprite.h"
class SpriteGL : public Sprite
{

	GLuint shaderID;
	GLint vertexAttribLoc;
	GLint uvAttribLoc;
	GLint matWorldUniformLoc;


	GLuint	IB;
	GLuint	VB;

	int IdTex;
	GLint IdTexUniformLoc;


public:

	void Create(Matrix4D &VP) ;
	void Draw() ;
	void Destroy() ;

	SpriteGL();
	~SpriteGL();
};
#endif

