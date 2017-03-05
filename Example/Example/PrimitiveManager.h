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
	int  CreateTriangle();
	int	 CreateCube();
	int CreateModel(char * fileName);

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);

	std::vector<PrimitiveBase*> primitives;

	Matrix4D *pVP;
};

#endif