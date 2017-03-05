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

	

	int indexScene = PrimitiveMgr.CreateModel("Models/Scene.X");
	PrimitiveMgr.GetPrimitive(indexScene)->Create();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexScene), &cam.VP);


	int indexHouse = PrimitiveMgr.CreateModel("Models/House.X");
	PrimitiveMgr.GetPrimitive(indexHouse)->Create();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexHouse), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().RotateYAbsolute(180);
	Models.back().TranslateAbsolute(200.f, 0, 200);
	Models.back().Update();

	int indexLink = PrimitiveMgr.CreateModel("Models/Link.X");
	PrimitiveMgr.GetPrimitive(indexLink)->Create();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexLink), &cam.VP);
	Models.back().ScaleAbsolute(0.3f);
	Models.back().TranslateAbsolute(75, 0, 70.0f);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	int indexDarius = PrimitiveMgr.CreateModel("Models/Darius.X");
	PrimitiveMgr.GetPrimitive(indexDarius)->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexDarius), &cam.VP);
	Models.back().ScaleAbsolute(7);
	Models.back().TranslateAbsolute(0.f, 0, 70.0f);
	Models.back().RotateYAbsolute(215);
	Models.back().Update();

	int indexJinx = PrimitiveMgr.CreateModel("Models/Jinx.X");
	PrimitiveMgr.GetPrimitive(indexJinx)->Create();
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexJinx), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-60.f, 0, 70.0f);
	Models.back().RotateYAbsolute(200);
	Models.back().Update();

	int indexModel = PrimitiveMgr.CreateModel("Models/NuBatman.X");
	PrimitiveMgr.GetPrimitive(indexModel)->Create();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexModel), &cam.VP);
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(100.f, 0, -100);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	int indexCerdo = PrimitiveMgr.CreateModel("Models/Cerdo.X");
	PrimitiveMgr.GetPrimitive(indexCerdo)->Create();


	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(50, 0, -200);
	Models.back().ScaleAbsolute(50);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(50, 110, -100);
	Models.back().ScaleAbsolute(15);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(0, 90, -100);
	Models.back().ScaleAbsolute(15);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCerdo), &cam.VP);
	Models.back().TranslateAbsolute(100, 90, -100);
	Models.back().ScaleAbsolute(15);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	int indexCroc = PrimitiveMgr.CreateModel("Models/NuCroc.X");
	PrimitiveMgr.GetPrimitive(indexCroc)->Create();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(indexCroc), &cam.VP);
	Models.back().TranslateAbsolute(0, 0, -100);
	Models.back().RotateYAbsolute(180);
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