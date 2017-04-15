#include "Application.h"
#include <iostream>
#include "MeshD3D.h"
#include "ModelGL.h"
#include "TextMeshD3D.h"
#include "SpriteGL.h"
#include"SpriteD3D.h"


void TestApp::InitVars() {
	Utils::Init();
	DtTimer.Init();

	cam.Init();

	SceneProp.AddCamera(&cam);
	SceneProp.AddLight(Vector3D(250, 25.0f, 120), Vector3D(1.0f, 0.8f, 0.8f), true);
	SceneProp.specExp = 14.0f;
	SceneProp.attMax = 12000.0f;
	//SceneProp.AmbientColor = Vector3D(0.15f, 0.15f, 0.15f);


	Matrix4D Projection = PerspectiveFOVRH(ToRadian(45.f), 1280.0f / 720.0f, 5.f, 1000.0f);
	Vector3D pos(0, 0, 5);
	Vector3D target(0, 0, 0);
	Vector3D up(0, 1, 0);
	Matrix4D View = LookAtRH(pos, target, up);
	Matrix4D VP = View* Projection;

#ifdef USING_GL_COMMON
	textFPS = new GLFont();
	sprite = new SpriteGL();
	dot = new SpriteGL();
#elif defined(USING_D3D11)
	textFPS = new TextMeshD3D();
	sprite = new SpriteD3D();
	dot = new SpriteD3D();

#endif

	sprite->SetTexture("radar.tga");
	sprite->Create(VP);//
	sprite->ScaleAbsolute(1);
	sprite->TranslateAbsolute(2, 1, 0);
	sprite->Update();

	dot->SetTexture("dot.tga");
	dot->Create(VP);//
	dot->ScaleAbsolute(0.05);
	dot->TranslateAbsolute(2.5, 1.5, 0);
	dot->Update();

	

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

	index = PrimitiveMgr.CreateModel("Models/CerdoNuevo.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(20);
	Models.back().RotateXAbsolute(90);
	Models.back().RotateZAbsolute(-30);
	Models.back().TranslateAbsolute(200, 25, 220);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/NuBatman.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(3);
	Models.back().TranslateAbsolute(200, 0, 140);
	Models.back().RotateYAbsolute(90);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/NuVenomJok.X");
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(200, 0, 90);
	Models.back().RotateYAbsolute(90);
	Models.back().Update();

	textFPS->Create();
	textFPS->TranslateAbsolute(-0.98,0.9,0);
	textFPS->Update();


	index = PrimitiveMgr.CreateModel("Models/CerdoNuevo.X");
	lightPrimitive.CreateInstance(PrimitiveMgr.GetPrimitive(index));
	lightPrimitive.ScaleAbsolute(3);
	lightPrimitive.RotateXAbsolute(90);
	lightPrimitive.RotateZAbsolute(-30);
	lightPrimitive.Update();



	PrimitiveMgr.SetSceneProps(&SceneProp);
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
}

void TestApp::OnUpdate() {
	DtTimer.Update();
	Vector3D campos = cam.m_pos / 1500.f;
	dot->TranslateAbsolute(2.44-campos.z,1.45-campos.x,0);
	dot->Update();

	OnInput();
	lightPrimitive.TranslateAbsolute(SceneProp.Lights[0].Position.x, SceneProp.Lights[0].Position.y, SceneProp.Lights[0].Position.z);
	lightPrimitive.Update();
	static float count = 0;
	count += DtTimer.GetDTSecs();
	if (count > 0.2f)
	{
		count = 0.0f;
		textFPS->m_text = "FPS: " + std::to_string(static_cast<int>(1 / DtTimer.GetDTSecs()));
		//std::cout << std::to_string(static_cast<int>(1 / DtTimer.GetDTSecs())) << std::endl;
	}
	OnDraw();
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();
	
	for (auto &it : Models)
		it.Draw();

	lightPrimitive.Draw();
	textFPS->Draw();
	dot->Draw();
	sprite->Draw();
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

	if (IManager.PressedKey(SDLK_r)) {
		SceneProp.Lights[0].Color = Vector3D(1,0.5,0.5);
	}
	if (IManager.PressedKey(SDLK_g)) {
		SceneProp.Lights[0].Color = Vector3D(0.5, 1, 0.5);
	}
	if (IManager.PressedKey(SDLK_b)) {
		SceneProp.Lights[0].Color = Vector3D(0.5, 0.5, 1);
	}

	//Light

	if (IManager.PressedKey(SDLK_KP4)) {
		SceneProp.Lights[0].Position.z += 100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP6)) {
		SceneProp.Lights[0].Position.z += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP8)) {
		SceneProp.Lights[0].Position.x += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP5)) {
		SceneProp.Lights[0].Position.x += 100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP7)) {
		SceneProp.Lights[0].Position.y += -100 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_KP9)) {
		SceneProp.Lights[0].Position.y += 100 * DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_z)) {
		SceneProp.specExp -= 20 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_x)) {
		SceneProp.specExp += 20 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_c)) {
		SceneProp.attMax -= 20000 * DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_v)) {
		SceneProp.attMax += 20000 * DtTimer.GetDTSecs();
	}

	//Render Mode
	if (IManager.PressedKey(SDLK_i)) {
		SceneProp.renderMode = RM::RenderMode::SOLID;
	}
	if (IManager.PressedKey(SDLK_o)) {
		SceneProp.renderMode = RM::RenderMode::WIREFRAME;
	}
	if (IManager.PressedKey(SDLK_p)) {
		SceneProp.renderMode = RM::RenderMode::SOLID_WIREFRAME;
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