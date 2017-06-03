#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H
#include "SceneProp.h"
struct CVertex {

	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
};
class PrimitiveBase {
public:
	bool useLight;
	virtual void Create() = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t) = 0;
	virtual void Destroy() = 0;

	virtual void SetShaderBySignature(unsigned long sig) = 0;
	void SetSceneProps(SceneProps *p) { pScProp = p; }
	SceneProps	*pScProp;
};


#endif