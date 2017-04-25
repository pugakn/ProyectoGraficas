#pragma once

#include "Config.h"
#include "BaseRT.h"
#if defined(USING_OPENGL_ES20)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3/gl31.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#else
#include <GL/glew.h>
#include <SDL/SDL.h>
#endif
class GLRT :
	public BaseRT
{
public:
	bool	Load(int nrt, int cf, int df, int w, int h) override;

	std::vector<GLuint>		vFrameBuffers;
	std::vector<GLuint>		vGLColorTex;
	GLuint					DepthTexture;
};

