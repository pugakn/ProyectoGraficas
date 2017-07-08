#pragma once
#include "BaseFX.h"
class BloomFX :
	public BaseFX
{
private:
	GLShader* illuminanceShader;
	GLShader* HorizontalBlurShader;
	GLShader* VerticalBlurShader;
	GLShader* SumShader;
public:
	void ApplyFX(Texture* deferredTexture) override;
	void InitAPI() override;
	BloomFX();
	~BloomFX();
};

