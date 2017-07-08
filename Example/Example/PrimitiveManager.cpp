
#include "PrimitiveManager.h"
#ifdef USING_GL_COMMON
#include "TriangleGL.h"
#include "ModelGL.h"
#elif defined(USING_D3D11)
#include "MeshD3D.h"
#endif
#include "CubeGL.h"
#include "Quad.h"
#include "ShaderManager.h"
PrimitiveBase*	PrimitiveManager::GetPrimitive(unsigned int index) {
	if (index >= primitives.size())
		return 0;

	return primitives[index];
}

void PrimitiveManager::SetShaderGlobalType(Shader::TYPE type)
{
	for (auto &it : primitives)
	{
		it->SetShaderType(type);
	}

}

int  PrimitiveManager::CreateTriangle() {
#ifdef USING_GL_COMMON
	PrimitiveBase *primitive = new TrangleGL();
//
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size()-1);
#endif;
	return -1;
}

int	 PrimitiveManager::CreateCube(){

	PrimitiveBase *primitive = new CubeGL();
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);

}

int	 PrimitiveManager::CreateModel(char * fileName, bool useLight) {
#ifdef USING_GL_COMMON
	PrimitiveBase *primitive = new ModelGL();
	primitive->useLight = useLight;
	dynamic_cast<ModelGL*>(primitive)->SetFileName(fileName);
	dynamic_cast<ModelGL*>(primitive)->Create();
#elif defined(USING_D3D11)
	PrimitiveBase *primitive = new MeshD3D();
	primitive->useLight = useLight;
	dynamic_cast<MeshD3D*>(primitive)->SetFileName(fileName);
	dynamic_cast<MeshD3D*>(primitive)->Create();
#endif
	
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}
#include "Plane.h"
int PrimitiveManager::CreatePlane()
{
	PrimitiveBase *primitive = new Plane();
	primitive->Create();
	primitives.push_back(primitive);
	return (int)(primitives.size() - 1);
}

int PrimitiveManager::CreateQuad()
{
	PrimitiveBase *primitive = new Quad();
	primitive->Create();
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
