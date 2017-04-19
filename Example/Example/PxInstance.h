#pragma once
#include "PrimitiveInstance.h"
#include <PxPhysicsAPI.h>
class PxInstance
{
protected:


public:
	physx::PxRigidDynamic *body;
	PrimitiveInst* primitive;
	virtual void Create(PrimitiveBase* prm, float scale) = 0;
	virtual void Update() = 0;
	virtual void Traslate(Vector3D trsl) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
	PxInstance();
	virtual ~PxInstance();
};

