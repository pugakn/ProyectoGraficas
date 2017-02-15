#include <Core.h>
#include <PrimitiveManager.h>
#include <PrimitiveInstance.h>
#include "Matrix4D.h"
#include "Vector3D.h"
#include <Timer.h>
#include "ModelGL.h"
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

	Vector3D		Position;
	Vector3D		Orientation;
	Vector3D		Scaling;

	Matrix4D		View;
	Matrix4D		Projection;
	Matrix4D		VP;

	Vector3D Pos;
	Vector3D Up;
	Vector3D LookAt;

	Timer			DtTimer;


};