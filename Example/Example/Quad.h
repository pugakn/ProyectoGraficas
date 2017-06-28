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
class Quad : public PrimitiveBase {
public:
	Quad()
#ifdef USING_GL_COMMON
		: shaderID(0)
#endif
	{
		difTex = nullptr;
		depthTex = nullptr;
		normalTex = nullptr;
		specTex = nullptr;
		shadowMapTexture = nullptr;
	}

#ifdef USING_D3D11
	struct CBuffer {
		Matrix4D World;
		Matrix4D VPInverse;
		Vector4D LightPositions[128];
		Vector4D LightColors[128];
		Vector4D CameraPosition;
		int NumLights;
		float ShadowTexSize[2];
		Matrix4D CamVP;
		int NumLights2;
	};
#endif

	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t) override;
	void Destroy();
	void SetShaderType(Shader::TYPE type) override;

#ifdef USING_GL_COMMON
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	diffuseLoc;
	GLint normalTextLoc;
	GLint specularTextLoc;
	GLint depthTextLoc;
	GLint   uvAttribLoc;


	GLint  matWorldUniformLoc;
	GLint WVPLoc;
	GLint WorldLoc;
	GLint WorldViewLoc;
	GLint VPInverseLoc;
	GLint LightPositionsLoc;
	GLint LightColorsLoc;
	GLint CameraPositionLoc;
	GLint NumLightsLoc;
	GLint ShadowMapSize;

	GLint LinearLightDirLoc;
	GLint CamVPLoc;
	GLint ShadowMapLoc;

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

	Quad::CBuffer	CnstBuffer;
#endif

	int		difTextId;
	Texture		*normalTex;
	int		normalTextId;
	Texture		*depthTex;
	int		depthTextId;
	Texture		*difTex;
	int		specTextId;
	Texture		*specTex;

	Texture		*shadowMapTexture;
	CVertex			vertices[4];
	unsigned short	indices[6];
	Matrix4D	transform;
};

