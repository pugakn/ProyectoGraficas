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
	model->m_bones = model->parser.bones;
	static int tick = 0;
	static int prevIndx = -1;
	static bool transf = true;
	static bool f = true;
	m_time += delta;
	tick += delta*4800;
	//if (m_time > 0.000208333f)
	//{
	//	tick++;
	//	m_time -= 0.000208333f;
	//}
	if (tick == prevIndx)
		return;
	prevIndx = tick;

	if (tick > 160 * animationSets[animIndex].animationsVec.size())
		tick = 0;

	Matrix4D firstcomb;
	static bool first = true;
	for (auto &anim : animationSets[animIndex].animationsVec) {
		


		//indx = indx%anim.maxIndex;
		Matrix4D scale;
		Matrix4D position;

		int scIndex = 0;
		int rotIndex = 0;
		int posIndex = 0;

		static int prevSc = 0;
		for (int i = prevSc; i < anim.scaleVec.size();i++)
		{
			if (tick < anim.scaleVec[i].ticks) {
				scIndex = i;
				break;
			}
		}

		static int prevTrans = 0;
		for (int i = prevTrans; i < anim.posVec.size(); i++)
		{
			if (tick < anim.posVec[i].ticks) {
				posIndex = i;
				break;
			}
		}


		static int prevRot = 0;
		for (int i = prevRot; i < anim.rotVec.size(); i++)
		{
			if (tick < anim.rotVec[i].ticks) {
				rotIndex = i;
				break;
			}
		}

		if (prevSc == scIndex && prevTrans == posIndex && prevRot == rotIndex)
			continue;
		prevRot = rotIndex;
		prevTrans = posIndex;
		prevSc = scIndex;


		//if (scIndex > anim.scaleVec.size())
		//	scIndex = anim.scaleVec.size() - 1;
		//if (rotIndex > anim.rotVec.size())
		//	rotIndex = anim.rotVec.size() - 1;
		//if (posIndex > anim.posVec.size())
		//	posIndex = anim.posVec.size() - 1;

		scale = Scaling(anim.scaleVec[scIndex].scale.x, anim.scaleVec[scIndex].scale.y, anim.scaleVec[scIndex].scale.z);
		Vector4D q (anim.rotVec[rotIndex].QRot.x, anim.rotVec[rotIndex].QRot.y, anim.rotVec[rotIndex].QRot.z, anim.rotVec[rotIndex].QRot.w);
		Matrix4D	Rotation;
		{
			float x2 = q.x * q.x;
			float y2 = q.y * q.y;
			float z2 = q.z * q.z;
			float xy = q.x * q.y;
			float xz = q.x * q.z;
			float yz = q.y * q.z;
			float wx = q.w * q.x;
			float wy = q.w * q.y;
			float wz = q.w * q.z;
			
			Matrix4D Temp(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
					2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
					2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
			
				
				for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Rotation.m[i][j] = Temp.m[i][j];
					
				}
			}
		}
		position =Translation(anim.posVec[posIndex].pos.x, anim.posVec[posIndex].pos.y, anim.posVec[posIndex].pos.z);
		Matrix4D comb = scale * Rotation * position;
		if (first) {
			first = false;
			firstcomb = comb;
		}

		if (comb != model->m_bones[anim.boneIndex].bone)
		{
			if (1) {
				model->SetNewTransforms(anim.boneIndex, comb);
				transf = true;
				//break;
			}

		}
		else {
			transf = false;
		}

		//break;
	}
	first = true;

	if (transf ) {
		f = false;
		model->TransformBone(0, firstcomb);
	}

}

void AnimationManager::SetAnim(int anim)
{
	animIndex = anim;
	m_time = 0;
}

void AnimationManager::ReadNodes()
{
}

void AnimationManager::Stop()
{
}

AnimationManager::AnimationManager()
{
	m_time = 0;
	animIndex = 0;
}
