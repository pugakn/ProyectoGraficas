#pragma once
#include "Config.h"
#ifdef USING_OPENGL_ES
#include "Sprite.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
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

