#pragma once
#include "Matrix4D.h"
#include "UtilsGL.h"
class Sprite
{
protected:
	Matrix4D transform;
	Matrix4D Position;
	Matrix4D Scale;
	float textureWidth;
	struct spriteVertex
	{
		float x, y, z, w;
		float u, v;
	};
	spriteVertex m_VBO[4];
	unsigned short m_indexBuffer[6];
public:
	void ScaleAbsolute(float sc);
	void TranslateAbsolute(float x, float y, float z);
	void SetTexture(char* str);
	void Update();

	virtual void Create(Matrix4D &VP) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;


	Matrix4D m_VP;
	char* m_str;
	Sprite();
	virtual ~Sprite();
};

