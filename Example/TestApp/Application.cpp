#include "Application.h"
#include <iostream>
#include "MeshD3D.h"
#include "ModelGL.h"


void TestApp::InitVars() {
	DtTimer.Init();

	cam.Init();

	SceneProp.AddCamera(&cam);
	SceneProp.AddLight(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.8f, 0.8f), true);
	//SceneProp.AmbientColor = Vector3D(0.15f, 0.15f, 0.15f);

}

void TestApp::CreateAssets() {	


	

	int index = PrimitiveMgr.CreateModel("Models/Scene.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));


	index = PrimitiveMgr.CreateModel("Models/House.X");

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-350, 0, -75);
	Models.back().Update();


	index = PrimitiveMgr.CreateModel("Models/Jinx.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(350, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Vi.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(250, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/grag.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(150, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ashe.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(50, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahri.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, 0);
	Models.back().RotateYAbsolute(180);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahriFire.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(350, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/amumu.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(250, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/anie.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(150, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Darius.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(10);
	Models.back().TranslateAbsolute(50, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/garen.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, -150);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/sona.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(2);
	Models.back().TranslateAbsolute(-50, 0, -75);
	Models.back().RotateYAbsolute(90);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Cerdo.X");
	lightPrimitive.CreateInstance(PrimitiveMgr.GetPrimitive(index));
	lightPrimitive.ScaleAbsolute(3);
	lightPrimitive.Update();
	

	PrimitiveMgr.SetSceneProps(&SceneProp);
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();

	OnInput();
	lightPrimitive.TranslateAbsolute(SceneProp.Lights[0].Position.x, SceneProp.Lights[0].Position.y, SceneProp.Lights[0].Position.z);
	lightPrimitive.Update();
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	for (auto &it : Models)
		it.Draw();

	lightPrimitive.Draw();

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

	//Light
	//if (IManager.PressedKey(SDLK_KP4)) {
	//	lightDir = lightDir*RotationY(-1 * DtTimer.GetDTSecs());
	//}
	//if (IManager.PressedKey(SDLK_KP6)) {
	//	lightDir = lightDir*RotationY(1 * DtTimer.GetDTSecs());
	//}
	//if (IManager.PressedKey(SDLK_KP8)) {
	//	lightDir = lightDir*RotationX(-1 * DtTimer.GetDTSecs());
	//}
	//if (IManager.PressedKey(SDLK_KP5)) {
	//	lightDir = lightDir*RotationX(1 * DtTimer.GetDTSecs());
	//}
	if (IManager.PressedKey(SDLK_KP4)) {
		SceneProp.Lights[0].Position.x += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP6)) {
		SceneProp.Lights[0].Position.x += 100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP8)) {
		SceneProp.Lights[0].Position.z += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP5)) {
		SceneProp.Lights[0].Position.z += 100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP7)) {
		SceneProp.Lights[0].Position.y += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP9)) {
		SceneProp.Lights[0].Position.y += 100 * DtTimer.GetDTSecs();
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