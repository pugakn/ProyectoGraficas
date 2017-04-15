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

#ifndef UAD_UTILS_GL_H
#define UAD_UTILS_GL_H

#include "Config.h"
#include <stdio.h>

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureGL.h"

bool checkcompilederrors(GLuint shader, GLenum type);
GLuint createShader(GLenum type, char* pSource);
#elif defined USING_D3D11
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif

char *file2string(const char *path);


class Utils
{
public:
	static Texture* textureCheker;
#ifdef USING_OPENGL_ES
public:
	static GLuint DefaultShaderID;
	static int textureChekerID;
#elif defined USING_D3D11
public:
	static ComPtr<ID3D11VertexShader>  pDefaultVS;
	static ComPtr<ID3D11PixelShader>   pDefaultFS;
	static ComPtr<ID3DBlob>  DefaultVS_blob;
	static 	ComPtr<ID3DBlob> DefaultFS_blob;
#endif
	Utils() {};
	~Utils() {};
	static void Init();

};


#endif