#include "Application.h"
#include <iostream>
#include "MeshD3D.h"
#include "ModelGL.h"


void TestApp::InitVars() {
	DtTimer.Init();

}

void TestApp::CreateAssets() {	
	cam.Init();
	PrimitiveMgr.SetVP(&cam.VP);

	

	int index = PrimitiveMgr.CreateModel("Models/Scene.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);


	index = PrimitiveMgr.CreateModel("Models/House.X");

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-350, 0, -75);
	Models.back().Update();


	index = PrimitiveMgr.CreateModel("Models/Jinx.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(350, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Vi.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(250, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/grag.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(150, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ashe.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(50, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahri.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahriFire.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(350, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/amumu.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(250, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/anie.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(150, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Darius.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(10);
	Models.back().TranslateAbsolute(50, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/garen.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/sona.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, -75);
	Models.back().RotateYAbsolute(90);
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
		cam.TraslateFront(-100.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_s)) {
		cam.TraslateFront(100.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_d)) {
		cam.TraslateSide(100.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_a)) {
		cam.TraslateSide(-100.0f*DtTimer.GetDTSecs());
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