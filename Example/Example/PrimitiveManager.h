#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include "Matrix4D.h"
#include "PrimitiveBase.h"
#include "Config.h"

class PrimitiveManager {
public:
	void SetVP(Matrix4D *vp) {
		pVP = vp;
	}
	void SetLightDir(Vector3D *l) {
		lightDir = l;
	}
	int  CreateTriangle();
	int	 CreateCube();
	int CreateModel(char * fileName);

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	Matrix4D *pVP;
	Vector3D *lightDir;
};

#endif