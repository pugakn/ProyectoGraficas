#include "FPCamera.h"
#include <math.h>
void FPCamera::Init()
{
	rotX = 0;
	rotY = 0;
	m_rotationVel = 2.0f;
	m_pos = Vector3D(0, 20, 0);
	Projection = PerspectiveFOVRH(ToRadian(45.f), 1280.0f / 720.0f, 1.f, 5000.0f);
	Update();
}

void FPCamera::TraslateFront(float velocity)
{
	m_pos = m_pos + velocity *zDir;
}
void FPCamera::TraslateSide(float velocity)
{
	m_pos = m_pos + velocity *xDir;
}

void FPCamera::RotateX(float rotation)
{
	//rotX += rotation * m_rotationVel;
	rotX = rotation * 3.1416f ;
}
void FPCamera::RotateY(float rotation)
{
	//rotY += rotation * m_rotationVel;
	rotY = rotation * 3.1416f + 1.5708;
}

void FPCamera::Update()
{
	float cosPitch = cos(rotX);
	float sinPitch = sin(rotX);
	float cosYaw = cos(rotY);
	float sinYaw = sin(rotY);

	//RotY * RotX
	xDir = Vector3D( cosYaw, 0, -sinYaw );
	yDir = Vector3D( sinYaw * sinPitch, cosPitch, cosYaw * sinPitch );
	zDir = Vector3D(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw );

	m_view = Matrix4D(xDir.x, yDir.x, zDir.x, 0,
		xDir.y, yDir.y, zDir.y, 0,
		xDir.z, yDir.z, zDir.z, 0,
		-Dot(xDir, m_pos), -Dot(yDir, m_pos), -Dot(zDir, m_pos), 1.f);
	VP = m_view*Projection;
}
void FPCamera::OnInput(float delta)
{
}



FPCamera::FPCamera()
{
}


FPCamera::~FPCamera()
{
}
