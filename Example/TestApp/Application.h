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
class TestApp : public AppBase {
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

	PrimitiveManager PrimitiveMgr;
	std::vector<PrimitiveInst>	Models;
	PrimitiveInst lightPrimitive;

	TextMesh* textFPS;


	FPCamera cam;
	//Vector3D lightPos;
	Timer			DtTimer;

	SceneProps		SceneProp;


};