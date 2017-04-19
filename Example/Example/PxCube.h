#pragma once
#include "PxInstance.h"
class PxCube :
	public PxInstance
{
public:
	void Create(PrimitiveBase* prm, float scale) override;
	void Update() override;
	void Draw() override;
	void Destroy()override;
	void Traslate(Vector3D trsl) override;
	PxCube();
	~PxCube();
};

