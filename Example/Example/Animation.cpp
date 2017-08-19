#include "Animation.h"
#include "Matrix4D.h"
#include "ModelGL.h"
#include "MeshD3D.h"

//xAnimation::xAnimation()
//{
//}
//
//
//xAnimation::~xAnimation()
//{
//}

Vector3D Lerp(Vector3D start, Vector3D end, float percent)
{
	return (start + percent*(end - start));
}

void AnimationManager::Play(float delta)
{
	static int tick = 0;
	m_time += delta;
	tick += delta * 48000;
	//if (m_time > 0.000208333f)
	//{
	//	tick++;
	//	m_time -= 0.000208333f;
	//}
	int indx = tick / 160;
	for (auto &anim : animationSets[animIndex].animationsVec) {
		indx = indx%anim.maxIndex;
		Matrix4D scale;
		Matrix4D rotation;
		Matrix4D position;

		int scIndex = indx;
		int rotIndex = indx;
		int posIndex = indx;
		if (scIndex > anim.scaleVec.size())
			scIndex = anim.scaleVec.size() - 1;
		if (rotIndex > anim.rotVec.size())
			rotIndex = anim.rotVec.size() - 1;
		if (posIndex > anim.posVec.size())
			posIndex = anim.posVec.size() - 1;

		scale = /*Scaling(anim.scaleVec[scIndex].scale.x, anim.scaleVec[scIndex].scale.y, anim.scaleVec[scIndex].scale.z)*/Identity();
		rotation = Identity();
		//XMATRIX44	Rotation;
		//+{
		//	+XQUATERNION q = pActualBone->ActualKey.RotationKey.Rot;
		//	+float x2 = q.x * q.x;
		//	+float y2 = q.y * q.y;
		//	+float z2 = q.z * q.z;
		//	+float xy = q.x * q.y;
		//	+float xz = q.x * q.z;
		//	+float yz = q.y * q.z;
		//	+float wx = q.w * q.x;
		//	+float wy = q.w * q.y;
		//	+float wz = q.w * q.z;
		//	+
		//		+XMATRIX44	Temp(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
		//			+2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
		//			+2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
		//			+0.0f, 0.0f, 0.0f, 1.0f);
		//	+
		//		+
		//		+for (int i = 0; i < 4; i++) {
		//		+for (int j = 0; j < 4; j++) {
		//			+Rotation.m[i][j] = Temp.m[i][j];
		//			+
		//		}
		//		+
		//	}
		//	+}
		position =/* Translation(anim.posVec[posIndex].pos.x, anim.posVec[posIndex].pos.y, anim.posVec[posIndex].pos.z)*/Identity();
		Matrix4D comb = scale * rotation * position;
		model->TransformBone(anim.boneIndex, comb);
		//break;
	}
}

void AnimationManager::SetAnim(int anim)
{
	animIndex = anim;
	m_time = 0;
}

void AnimationManager::Stop()
{
}

AnimationManager::AnimationManager()
{
	m_time = 0;
	animIndex = 0;
}
