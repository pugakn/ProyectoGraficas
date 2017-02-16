#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>
#include "Matrix4D.h"
#include "Vector3D.h"
#include <Timer.h>
#include "ModelGL.h"
#include "FPCamera.h"
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
	PrimitiveInst	Models[10];



	FPCamera cam;

	Timer			DtTimer;


};