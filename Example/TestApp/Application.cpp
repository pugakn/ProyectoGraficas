#include "Application.h"
#include "BMPParser.h"


void TestApp::InitVars() {
	DtTimer.Init();
	Position	= Vector3D(0.0f, 0.0f, 0.0f);
	Orientation = Vector3D(0.0f, 0.0f, 0.0f);
	Scaling		= Vector3D(1.0f, 1.0f, 1.0f);
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.SetVP(&VP);
	int indexModel = PrimitiveMgr.CreateModel();
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexModel))->SetFileName("DealerA14.X");
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexModel))->Create();
	Models[0].CreateInstance(PrimitiveMgr.GetPrimitive(indexModel), &VP);

	Pos	= Vector3D(0.0f,1.0f,5.0f);
	Up = Vector3D(0.0f, 1.0f, 0.0f);
	LookAt = Vector3D(0.0001f, 0.0001f, 0.0001f) - Pos;
	View = LookAtRH(Pos, LookAt, Up);

	Projection = PerspectiveFOVRH(ToRadian(45.f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	VP = View*Projection;

	BMPParser bp;
	bp.LoadFile("bmp.bmp");


}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();

	Models[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	Models[0].RotateXAbsolute(Orientation.x);
	Models[0].RotateYAbsolute(Orientation.y);
	Models[0].RotateZAbsolute(Orientation.z);
	Models[0].ScaleAbsolute(Scaling.x);
	Models[0].Update();

	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	Models[0].Draw();

	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	
	if (IManager.PressedKey(SDLK_UP)) {
		Position.y += 10.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.y -= 10.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 10.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 10.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		Position.z -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		Position.z += 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PLUS)) {
		Scaling.x += 1.0f*DtTimer.GetDTSecs();
		Scaling.y += 1.0f*DtTimer.GetDTSecs();
		Scaling.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_MINUS)) {
		Scaling.x -= 1.0f*DtTimer.GetDTSecs();
		Scaling.y -= 1.0f*DtTimer.GetDTSecs();
		Scaling.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		Orientation.x -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		Orientation.x += 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		Orientation.y -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP3)) {
		Orientation.y += 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		Orientation.z -= 60.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		Orientation.z += 60.0f*DtTimer.GetDTSecs();
	}

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}