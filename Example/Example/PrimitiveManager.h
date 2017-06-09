#ifndef UAD_PRIMITIVEMANAGER_H
#define UAD_PRIMITIVEMANAGER_H

#include <vector>
#include "Matrix4D.h"
#include "PrimitiveBase.h"
#include "Config.h"
#include "SceneProp.h"

class PrimitiveManager {
public:
	//void SetVP(Matrix4D *vp) {
	//	pVP = vp;
	//}
	//void SetLightDir(Vector3D *l) {
	//	lightDir = l;
	//}
	int  CreateTriangle();
	int	 CreateCube();
	int CreateModel(char * fileName , bool useLight);
	int CreateText();
	int CreatePlane();
	int CreateQuad();

	void DrawPrimitives();
	void DestroyPrimitives();
	PrimitiveBase*	GetPrimitive(unsigned int);
	void SetShaderGlobalType(Shader::TYPE type);

	std::vector<PrimitiveBase*> primitives;

	void SetSceneProps(SceneProps *p);

	//Matrix4D *pVP;
	//Vector3D *lightDir;
};

#endif