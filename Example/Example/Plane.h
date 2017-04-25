#pragma once
#include "PrimitiveBase.h"
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

#include "Config.h"

#include "UtilsGL.h"

#ifdef USING_OPENGL_ES20
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#include "TextureGL.h"
#elif defined(USING_OPENGL_ES30)
#include <GLES3\gl31.h>
#include "TextureGL.h"
#elif defined(USING_OPENGL)
#include <GL\glew.h>
#include "TextureGL.h"
#elif defined(USING_D3D11)
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif

#include "Matrix4D.h"
#include "PrimitiveBase.h"
class Plane : public PrimitiveBase {
public:
	Plane()
#ifdef USING_GL_COMMON
		: shaderID(0)
#endif
	{}

#ifdef USING_D3D11
	struct CBuffer {
		Matrix4D WVP;
	};
#endif

	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t) override;
	void Destroy();

#ifdef USING_GL_COMMON
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	diffuseLoc;


	GLint  matWorldViewProjUniformLoc;

	GLuint			VB;
	GLuint			IB;
#elif defined(USING_D3D11)
	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	Plane::CBuffer	CnstBuffer;
#endif

	int		TexId;
	CVertex			vertices[4];
	unsigned short	indices[6];
	Matrix4D	transform;
	Texture		*tex;
};

