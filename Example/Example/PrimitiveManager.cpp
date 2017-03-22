
#include "PrimitiveManager.h"
#ifdef USING_OPENGL_ES
#include "TriangleGL.h"
#include "CubeGL.h"
#include "ModelGL.h"
#elif defined(USING_D3D11)
#include "MeshD3D.h"
#endif

PrimitiveBase*	PrimitiveManager::GetPrimitive(unsigned int index) {
	if (index >= primitives.size())
		return 0;

	return primitives[index];
}

int  PrimitiveManager::CreateTriangle() {
#ifdef USING_OPENGL_ES
	PrimitiveBase *primitive = new TrangleGL();
//
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size()-1);
#endif;
	return -1;
}

int	 PrimitiveManager::CreateCube(){
#ifdef USING_OPENGL_ES
	PrimitiveBase *primitive = new CubeGL();
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
#endif;
	return -1;
}

int	 PrimitiveManager::CreateModel(char * fileName) {
#ifdef USING_OPENGL_ES
	PrimitiveBase *primitive = new ModelGL();
	dynamic_cast<ModelGL*>(primitive)->SetFileName(fileName);
	dynamic_cast<ModelGL*>(primitive)->Create();
#elif defined(USING_D3D11)
	PrimitiveBase *primitive = new MeshD3D();
	dynamic_cast<MeshD3D*>(primitive)->SetFileName(fileName);
	dynamic_cast<MeshD3D*>(primitive)->Create();
#endif
	
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}

void PrimitiveManager::DrawPrimitives() {
	for(unsigned int i=0;i<primitives.size();i++){
		primitives[i]->Draw(0);
	}
}

void PrimitiveManager::DestroyPrimitives() {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->Destroy();
		delete primitives[i];
	}
	primitives.clear();
}
void PrimitiveManager::SetSceneProps(SceneProps *p) {
	for (unsigned int i = 0; i < primitives.size(); i++) {
		primitives[i]->SetSceneProps(p);
	}
}
