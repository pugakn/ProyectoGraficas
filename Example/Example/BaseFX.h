#pragma once
#include "ShaderManager.h"
#include "UtilsGL.h"
#include "GLShader.h"
#include "PrimitiveBase.h"
class BaseFX
{
protected:
	Texture* m_deferredTexture;
	GLuint			VB;
	GLuint			IB;
	CVertex			vertices[4];
	unsigned short	indices[6];
public:

	enum FX_TYPE
	{
		BLOOM = 1
	};
	BaseFX();
	virtual ~BaseFX();
	void Init();
	virtual void InitAPI() = 0;
	virtual void ApplyFX(Texture* deferredTexture) = 0;
};

