#pragma once
#include "BaseFX.h"

class BloomFX :
	public BaseFX
{
private:
#ifndef USING_D3D11
	GLShader* illuminanceShader;
	GLShader* HorizontalBlurShader;
	GLShader* VerticalBlurShader;
	GLShader* SumShader;
#else

#endif
public:
	void ApplyFX(Texture* deferredTexture) override;
	void InitAPI() override;
	BloomFX();
	~BloomFX();
};


