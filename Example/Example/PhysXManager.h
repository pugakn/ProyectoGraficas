#pragma once
#include <PxPhysicsAPI.h>
class PhysXManager
{
	float m_fAccumulator;
	const float STEP_TIME = 1.0f / 120.0f;
public:
	PhysXManager(){ 
		m_fAccumulator = 0.0f; 
	}
	void Init();
	void Step(float delta);
};