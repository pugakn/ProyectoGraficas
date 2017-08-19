#pragma once
#include "Vector4D.h"
#include <vector>
#include <string>
#include "Config.h"
struct xAnimationKeyRot {
	Vector4D QRot;
	int ticks;
};
struct xAnimationKeyPos {
	Vector3D pos;
	int ticks;
};
struct xAnimationKeyScale {
	Vector3D scale;
	int ticks;
};
struct xAnimation
{
	std::string name;
	int boneIndex;
	int maxIndex;
	std::vector<xAnimationKeyRot> rotVec;
	std::vector<xAnimationKeyPos> posVec;
	std::vector<xAnimationKeyScale> scaleVec;
};
struct xAnimationSet {
	std::string name;
	int ticksPS;
	std::vector<xAnimation> animationsVec;
};


class ModelGL;
class MeshD3D;
class AnimationManager {
private:
	float m_time;
	int animIndex;
public:
#ifdef USING_D3D11
	MeshD3D* model;
#else
	ModelGL* model;
#endif

	void Play( float delta);
	void SetAnim(int anim);
	void Stop();
	AnimationManager();

	std::vector<xAnimationSet> animationSets;
};

