#pragma once
#include "Vector4D.h"
#include "Matrix4D.h"
#include "InputManager.h"
class FPCamera
{
private:
	Matrix4D Projection;
	InputManager IManager;

	Vector3D zDir;
	Vector3D xDir;
	Vector3D yDir;

	float rotY, rotX;

	Vector3D up;

public:
	void Init();
	void TraslateFront(float velocity);
	void TraslateSide(float velocity);

	void RotateX(float rotation);
	void RotateY(float rotation);
	void OnInput(float delta);
	void Update();

	Vector3D m_pos;
	Matrix4D m_view;
	Matrix4D VP;
	FPCamera();
	~FPCamera();
};

