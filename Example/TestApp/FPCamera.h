#pragma once
#include "Vector4D.h"
#include "Matrix4D.h"
#include "InputManager.h"
class FPCamera
{
private:
	Matrix4D m_rot;
	Matrix4D Projection;
	InputManager IManager;

public:
	void Init();
	void Traslate(Vector4D traslation);
	void Rotate(Vector4D rotation);
	void OnInput(float delta);

	Vector4D m_pos;
	Matrix4D m_view;
	Matrix4D VP;
	FPCamera();
	~FPCamera();
};

