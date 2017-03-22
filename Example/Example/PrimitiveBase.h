#ifndef UAD_PRIMITIVEBASE_H
#define UAD_PRIMITIVEBASE_H
#include "SceneProp.h"
class PrimitiveBase {
public:
	virtual void Create() = 0;
	virtual void Transform(float *t) = 0;
	virtual void Draw(float *t) = 0;
	virtual void Destroy() = 0;

	void SetSceneProps(SceneProps *p) { pScProp = p; }
	SceneProps	*pScProp;
};


#endif