#include "FPCamera.h"
#include <math.h>
void FPCamera::Init()
{
	rotX = 0;
	rotY = 3.1416;
	Vector3D initTarget(0, 5, 20);
	m_pos = Vector3D(0.0f, 5.0f, -20.0f);
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
	rotX += rotation;
}
void FPCamera::RotateY(float rotation)
{
	rotY += rotation;
}

void FPCamera::Update()
{
	float cosPitch = cos(rotX);
	float sinPitch = sin(rotX);
	float cosYaw = cos(rotY);
	float sinYaw = sin(rotY);

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
