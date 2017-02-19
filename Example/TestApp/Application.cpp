#include "Application.h"
#include "BMPParser.h"
#include <iostream>


void TestApp::InitVars() {
	DtTimer.Init();

}

void TestApp::CreateAssets() {	
	cam.Init();
	PrimitiveMgr.SetVP(&cam.VP);

	int indexModel = PrimitiveMgr.CreateModel();
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexModel))->SetFileName("NuBatman.X");
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexModel))->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexModel), &cam.VP);

	int indexCerdo = PrimitiveMgr.CreateModel();
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCerdo))->SetFileName("Cerdo.X");
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCerdo))->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);


	//BMPParser bp;
	//bp.LoadFile("bmp2.bmp");
	////for (int i = 0; i < bp.bmpSize;i++) {
	//	//if (bp.m_bmpData[0] == 255)
	//	//	std::cout << "R";
	////}

	//bp.Deallocate();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();

	//Models[0].TranslateAbsolute(Position.x, Position.y, Position.z);
	//Models[0].RotateXAbsolute(Orientation.x);
	//Models[0].RotateYAbsolute(Orientation.y);
	//Models[0].RotateZAbsolute(Orientation.z);
	//Models[0].ScaleAbsolute(Scaling.x);
	//Models[0].Update();

	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	for (auto &it : Models)
		it.Draw();

	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	//cam.OnInput(DtTimer.GetDTSecs());
	if (IManager.PressedKey(SDLK_UP)) {
		cam.Traslate(Vector4D(0, 3.0f*DtTimer.GetDTSecs(), 0, 1));
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		cam.Traslate(Vector4D(0, -3.0f*DtTimer.GetDTSecs(), 0, 1));
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		cam.Traslate(Vector4D(-3.0f*DtTimer.GetDTSecs(), 0, 0, 1));
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		cam.Traslate(Vector4D(3.0f*DtTimer.GetDTSecs(), 0, 0, 1));
	}

	if (IManager.PressedKey(SDLK_z)) {
		cam.Traslate(Vector4D(0, 0, -10.0f*DtTimer.GetDTSecs(), 1));
	}

	if (IManager.PressedKey(SDLK_x)) {
		cam.Traslate(Vector4D(0, 0, 10.0f*DtTimer.GetDTSecs(), 1));
	}

	if (IManager.PressedKey(SDLK_KP5)) {
		cam.Rotate(Vector4D(-1.0f*DtTimer.GetDTSecs(), 0, 0, 1));
	}

	if (IManager.PressedKey(SDLK_KP6)) {
		cam.Rotate(Vector4D(1.0f*DtTimer.GetDTSecs(), 0, 0, 1));
	}

	if (IManager.PressedKey(SDLK_KP2)) {
		cam.Rotate(Vector4D(0, -1.0f*DtTimer.GetDTSecs(), 0, 1));	}

	if (IManager.PressedKey(SDLK_KP3)) {
		cam.Rotate(Vector4D(0, 1.0f*DtTimer.GetDTSecs(), 0, 1));
	}

	if (IManager.PressedKey(SDLK_KP0)) {
		cam.Rotate(Vector4D(0, 0, -1.0f*DtTimer.GetDTSecs(), 1));
	}

	if (IManager.PressedKey(SDLK_KP_PERIOD)) {
		cam.Rotate(Vector4D(0, 0, 1.0f*DtTimer.GetDTSecs(), 1));
	}

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}