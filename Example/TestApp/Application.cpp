#include "Application.h"
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
	Models.back().ScaleAbsolute(0.2);
	Models.back().TranslateAbsolute(5.f,0,0);
	Models.back().Update();

	int indexCerdo = PrimitiveMgr.CreateModel();
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCerdo))->SetFileName("Cerdo.X");
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCerdo))->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(0, 0, 2.5);
	Models.back().ScaleAbsolute(5);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(0, 11, 2.5);
	Models.back().ScaleAbsolute(0.5);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(-5, 9, 0);
	Models.back().ScaleAbsolute(0.5);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(5, 9, 0);
	Models.back().ScaleAbsolute(0.5);
	Models.back().Update();

	int indexCroc = PrimitiveMgr.CreateModel();
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCroc))->SetFileName("NuCroc.X");
	dynamic_cast<ModelGL*>(PrimitiveMgr.GetPrimitive(indexCroc))->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCroc), &cam.VP);
	Models.back().ScaleAbsolute(0.1);
	Models.back().TranslateAbsolute(-5.f, 0, 0);
	Models.back().Update();
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();

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
	if (IManager.PressedKey(SDLK_w)) {
		cam.TraslateFront(-10.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_s)) {
		cam.TraslateFront(10.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_d)) {
		cam.TraslateSide(10.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_a)) {
		cam.TraslateSide(-10.0f*DtTimer.GetDTSecs());
	}




	Vector3D mRot = IManager.m_normalizedMousePos;

	cam.RotateY(-(mRot.x * 2 - 1));
	cam.RotateX(-(mRot.y * 2 - 1));
	cam.Update();

	
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}