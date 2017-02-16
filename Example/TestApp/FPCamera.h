#pragma once
#include "Vector4D.h"
#include "Matrix4D.h"
class FPCamera
{
private:
	Matrix4D m_rot;
	Matrix4D Projection;

public:
	void Init();
	void Traslate(Vector4D traslation);
	void Rotate(Vector4D rotation);

	Vector4D m_pos;
	Matrix4D m_view;
	Matrix4D VP;
	FPCamera();
	~FPCamera();
};

