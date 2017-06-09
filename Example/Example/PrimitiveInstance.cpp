#include "PrimitiveInstance.h"
#include "ShaderManager.h"
void PrimitiveInst::TranslateAbsolute(float x, float y, float z){
	Position = Translation(x, y, z);
}

void PrimitiveInst::RotateXAbsolute(float ang) {
	m_RotationX = RotationX(ToRadian(ang));
}

void PrimitiveInst::RotateYAbsolute(float ang) {
	m_RotationY = RotationY(ToRadian(ang));
}

void PrimitiveInst::RotateZAbsolute(float ang) {
	m_RotationZ = RotationZ(ToRadian(ang));
}

void PrimitiveInst::ScaleAbsolute(float sc) {
	Scale = Scaling(sc,sc,sc);
}

void PrimitiveInst::TranslateRelative(float x, float y, float z) {
	Matrix4D tmp =  Translation( x, y, z);
	Position = Position * tmp;
}

void PrimitiveInst::RotateXRelative(float ang) {
	Matrix4D tmp = RotationX(ToRadian(ang));
	m_RotationX = m_RotationX * tmp;
}

void PrimitiveInst::RotateYRelative(float ang) {
	Matrix4D tmp = RotationX(ToRadian(ang));
	m_RotationY = m_RotationY * tmp;
}

void PrimitiveInst::RotateZRelative(float ang) {
	Matrix4D tmp = RotationX(ToRadian(ang));
	m_RotationZ = m_RotationZ * tmp;
}

void PrimitiveInst::ScaleRelative(float sc) {
	Matrix4D tmp = Scaling(sc, sc, sc);
	Scale =  Scale * tmp;
}

void PrimitiveInst::Update() {
	Final = Scale*m_RotationX*m_RotationY*m_RotationZ*Position;

}

void PrimitiveInst::Draw(){
	pBase->Draw(&Final.m[0][0]);
}

void PrimitiveInst::SetShaderType(Shader::TYPE type)
{
	//ShaderManager::SetGlobalSignature(gsig);
	pBase->SetShaderType(type);
}
