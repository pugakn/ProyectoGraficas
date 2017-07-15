#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>
#include "Matrix4D.h"
#include "Vector3D.h"
#include <Timer.h>
#include "FPCamera.h"
#include <vector>

#include "GLFont.h"
#include "TextMeshD3D.h"

#include <SceneProp.h>

#include "Sprite.h"
#include "PhysXManager.h"
#include "PxCube.h"
#include<thread>
class TestApp : public AppBase {
private: 
	int deferredTexture;
public:
	TestApp() : AppBase() {}
	void InitVars();
	void CreateAssets();
	void DestroyAssets();

	void OnUpdate();
	void OnDraw();
	void OnInput();

	void OnPause();
	void OnResume();

	void OnReset();

	void LoadModel(char* file , Vector3D translate, float scale);
	std::vector<std::thread*> loadThreads;

	PrimitiveManager PrimitiveMgr;
	std::vector<PrimitiveInst>	Models;
	std::vector<PrimitiveInst>	DebugRT;
	PrimitiveInst lightPrimitive;

	TextMesh* textFPS;


	FPCamera cam;
	//Vector3D lightPos;
	Timer			DtTimer;

	SceneProps		SceneProp;

	Sprite* dot;
	Sprite* sprite;

	std::vector<PxInstance*> Cubes;

	PhysXManager physxManager;
	bool usePhysX;
};