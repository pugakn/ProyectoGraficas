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

#ifdef USING_GL_COMMON
#ifdef USING_OPENGL_ES20
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3\gl31.h>
#elif defined(USING_OPENGL)
#include <GL\glew.h>
#endif

#include "TextureGL.h"
class GLDriver;
bool checkcompilederrors(GLuint shader, GLenum type);
GLuint createShader(GLenum type, char* pSource);
#elif defined USING_D3D11
class D3DXDriver;
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif
#include <vector>
class BaseRT;
class BaseDriver;

char *file2string(const char *path);
class Tools
{
private:
	static std::vector<Texture*> m_textures;
public:
	static int CreateRT(int numRT, int colorf, int depthf, int w, int h);
	static int CreateRT(int numRT);
	static void PushRT(int id);
	static void PopRT();
	static void DestroyRTs();
	static Texture* textureCheker;
	static std::vector<BaseRT*> RTs;
#ifdef USING_GL_COMMON
private:
	static GLDriver* pVideoDriver;
public:
	static GLuint DefaultShaderID;
	static int textureChekerID;
#elif defined USING_D3D11
private:
	static D3DXDriver* pVideoDriver;
public:
	static ComPtr<ID3D11VertexShader>  pDefaultVS;
	static ComPtr<ID3D11PixelShader>   pDefaultFS;
	static ComPtr<ID3DBlob>  DefaultVS_blob;
	static ComPtr<ID3DBlob> DefaultFS_blob;
#endif
	static int LoadTexture(const char* path);
	static Texture* GetTexture(int id);
	Tools() {};
	~Tools() {};
	static void Init(BaseDriver* driver);

};


#endif