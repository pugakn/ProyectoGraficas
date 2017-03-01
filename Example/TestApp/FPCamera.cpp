#include "FPCamera.h"
#include <math.h>
void FPCamera::Init()
{
	Vector3D initTarget(0, 5, 20);
	m_pos = Vector3D(1.0f, 5.0f, -20.0f);
	up = Vector3D(0,1,0);
	zDir = Normalize( m_pos - initTarget);
	xDir = Normalize(Cross3(up,zDir));
	yDir = Cross3(zDir, xDir);

	m_view =  Matrix4D(xDir.x, yDir.x, zDir.x, 0,
			xDir.y, yDir.y, zDir.y, 0,
			xDir.z, yDir.z, zDir.z, 0,
			-Dot(xDir, m_pos), -Dot(yDir, m_pos), -Dot(zDir, m_pos), 1.f);

	Projection = PerspectiveFOVRH(ToRadian(45.f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	VP = m_view*Projection;
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
	Matrix4D rotationsMTX = RotationX(rotation);
	zDir = zDir * rotationsMTX;
	xDir = xDir * rotationsMTX;
	yDir = yDir * rotationsMTX;
}
void FPCamera::RotateY(float rotation)
{
	Matrix4D rotationsMTX = RotationY(rotation);
	zDir = zDir * rotationsMTX;
	xDir = xDir * rotationsMTX;
	yDir = yDir * rotationsMTX;
}

void FPCamera::Update()
{
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
