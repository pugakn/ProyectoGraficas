#include "FPCamera.h"



void FPCamera::Init()
{
	m_pos = Vector4D(0.0f, -5.0f, -20.0f,1.f);
	m_rot = Identity();
	m_rot = RotationY(ToRadian(180));
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

void FPCamera::OnInput(float delta)
{
	//if (IManager.PressedKey(SDLK_UP)) {
	//	Traslate(Vector4D(0, 1.0f*delta, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_DOWN)) {
	//	Traslate(Vector4D(0, -1.0f*delta, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_LEFT)) {
	//	Traslate(Vector4D(-1.0f*delta, 0, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_RIGHT)) {
	//	Traslate(Vector4D(1.0f*delta, 0, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_z)) {
	//	Traslate(Vector4D(0, 0, -10.0f*delta, 1));
	//}

	//if (IManager.PressedKey(SDLK_x)) {
	//	Traslate(Vector4D(0, 0, 10.0f*delta, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP5)) {
	//	Rotate(Vector4D(-1.0f*delta, 0, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP6)) {
	//	Rotate(Vector4D(1.0f*delta, 0, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP2)) {
	//	Rotate(Vector4D(0, -1.0f*delta, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP3)) {
	//	Rotate(Vector4D(0, 1.0f*delta, 0, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP0)) {
	//	Rotate(Vector4D(0, 0, -1.0f*delta, 1));
	//}

	//if (IManager.PressedKey(SDLK_KP_PERIOD)) {
	//	Rotate(Vector4D(0, 0, 1.0f*delta, 1));
	//}
}

FPCamera::FPCamera()
{
}


FPCamera::~FPCamera()
{
}
