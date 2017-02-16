#include "FPCamera.h"



void FPCamera::Init()
{
	m_pos = Vector4D(0.0f, -1.0f, -5.0f,1.f);
	m_rot = Identity();
	m_view = m_rot*Translation(m_pos.x, m_pos.y, m_pos.z);
	Projection = PerspectiveFOVRH(ToRadian(45.f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	VP = m_view*Projection;
}

void FPCamera::Traslate(Vector4D traslation)
{
	Matrix4D traslationsMTX = Translation(-traslation.x, -traslation.y, -traslation.z);
	m_pos = m_pos * traslationsMTX;
	m_view = m_rot*Translation(m_pos.x, m_pos.y, m_pos.z);
	VP = m_view*Projection;

}

void FPCamera::Rotate(Vector4D rotation)
{
	Matrix4D rotationsMTX = RotationX(rotation.x)
		* RotationY(rotation.y)* RotationZ(rotation.z);
	m_rot = m_rot * rotationsMTX;
	m_view = m_rot*Translation(m_pos.x, m_pos.y, m_pos.z);
	VP = m_view*Projection;
}

FPCamera::FPCamera()
{
}


FPCamera::~FPCamera()
{
}
