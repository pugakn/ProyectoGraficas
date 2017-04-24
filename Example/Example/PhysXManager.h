#pragma once
#include <PxPhysicsAPI.h>
class PhysXManager
{
	float m_fAccumulator;
	const float STEP_TIME = 1.0f / 60.0f;
public:
	PhysXManager(){ 
		m_fAccumulator = 0.0f; 
	}
	void Init();
	bool Step(float delta);
};