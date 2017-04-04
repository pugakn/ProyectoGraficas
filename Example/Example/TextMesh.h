#pragma once
#include "Matrix4D.h"
#include "Font.h"
#include "UtilsGL.h"
#include <string>
class TextMesh
{
protected:
	Matrix4D transform;
	Font font;
	//================
	Matrix4D		Position;
	Matrix4D		Scale;
	float textureWidth;
	struct fontVertex
	{
		float x, y, z, w;
		float u, v;
	};
	fontVertex m_VBO[4];
	unsigned short m_indexBuffer[6];

public:
	std::string m_text;

	void TranslateAbsolute(float x, float y, float z);
	virtual void Update();
	virtual void Create() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
	TextMesh();
	virtual ~TextMesh();
};

