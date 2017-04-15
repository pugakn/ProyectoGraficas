/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#ifndef UAD_GLDRIVER_H
#define UAD_GLDRIVER_H

#include "Config.h"

#include "BaseDriver.h"

#if defined(USING_OPENGL_ES20)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <EGL/egl.h>
#include <GLES3/gl31.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#include <SDL/SDL.h>
#else
#include <GL/glew.h>
#include <SDL/SDL.h>
#endif
#include <vector>
class GLDriver : public BaseDriver {
public:
	GLDriver() {  }
	void	InitDriver();
	void	CreateSurfaces();
	void	DestroySurfaces();
	void	Update();
	void	DestroyDriver();
	void	SetWindow(void *window);
	void	SetDimensions(int, int);

	void	Clear();
	void	SwapBuffers();
#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)
	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

	EGLNativeWindowType	eglWindow;
#endif
	GLint				CurrentFBO;
#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30)
	GLenum				DrawBuffers[16];
#endif
	int	width, height;
	std::vector<std::string>	ExtensionsTok;
	std::string					Extensions;
};

#endif