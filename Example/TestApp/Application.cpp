#include "Application.h"
#include <iostream>
#include "MeshD3D.h"
#include "ModelGL.h"
#include "TextMeshD3D.h"
#include "SpriteGL.h"
#include"SpriteD3D.h"
#include "Plane.h"
#include "BaseRT.h"
///////////////////////////////////////////////////////////////////////////
//NOTA: PRESIONAR L PARA COMENZAR O PAUSAR LA FISICAS                            <-----------------------------!!
//////////////////////////////////////////////////////////////////////////
using namespace physx;
extern PxFoundation *g_Foundation;
extern PxPhysics* g_Physics;
extern PxScene *g_scene;
void TestApp::InitVars() {
	physxManager.Init();
	usePhysX = false;
	//Material
	PxMaterial* material = g_Physics->createMaterial(1, 1, 0.0);
	//Actors
	PxTransform planePos(PxVec3(0, 0, 0), PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxRigidStatic* plane = g_Physics->createRigidStatic(planePos);
	plane->createShape(PxPlaneGeometry(), *material);
	g_scene->addActor(*plane);




	Tools::Init(pFramework->pVideoDriver);
	Tools::CreateRT(1);
	DtTimer.Init();
	cam.Init();

	SceneProp.AddCamera(&cam);
	SceneProp.AddLight(Vector3D(0, 25.0f, -270), Vector3D(1.0f, 0.8f, 0.8f), true);
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
	

	int index = PrimitiveMgr.CreateModel("Models/Scene.X",true);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));

	index = PrimitiveMgr.CreateModel("Models/Jinx.X",false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(100, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Vi.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-50, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/grag.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-100, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ashe.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(150, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahri.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-150, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/ahriFire.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(200, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/amumu.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-200, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/anie.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(250, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/Darius.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(3);
	Models.back().TranslateAbsolute(-250, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/garen.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(300, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateModel("Models/sona.X", false);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-300, 0, -360);
	Models.back().Update();

	index = PrimitiveMgr.CreateCube();

	for (size_t k = 0; k < 5; k++)
	{
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 7; j++)
			{
				Cubes.push_back(new PxCube());
				Cubes.back()->Create(PrimitiveMgr.GetPrimitive(index), 0.6);
				Cubes.back()->Traslate(Vector3D( i * 4 , 100 + j * 100 , k * 4 ));
				Cubes.back()->body->setMass(20.0f);
				//Cubes.back()->body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
				//Cubes.back()->body->setAngularVelocity(PxVec3(0.f, 2.f, 0.0));
				//Cubes.back()->body->setMassSpaceInertiaTensor(PxVec3(7.f, 7.f, 7.f));
			}
		}
	}

	for (size_t k = 0; k < 4; k++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				Cubes.push_back(new PxCube());
				Cubes.back()->Create(PrimitiveMgr.GetPrimitive(index), 2);
				Cubes.back()->Traslate(Vector3D(i * 4.5, 4.5 + j * 4.5, k * 4.5));
				Cubes.back()->body->setMass(40.0f);
			}
		}
	}

	index = PrimitiveMgr.CreateModel("Models/NuBatman.X", true);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(0.5);
	Models.back().TranslateAbsolute(0, 0, -360);
	Models.back().Update();


	index = PrimitiveMgr.CreateModel("Models/NuVenomJok.X",true);
	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().ScaleAbsolute(0.5);
	Models.back().TranslateAbsolute(50, 0, -360);
	Models.back().Update();


	//==================== Reflejo ===========================
	index = PrimitiveMgr.CreatePlane();
	dynamic_cast<Plane*>(PrimitiveMgr.primitives[index])->tex = Tools::RTs[0]->vColorTextures[0];

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-1500, 1, -380);
	Models.back().RotateXAbsolute(90);
	Models.back().ScaleAbsolute(3000);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-1500, 1, 3380);
	Models.back().RotateXAbsolute(90);
	Models.back().ScaleAbsolute(3000);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(380, 1, 1500);
	Models.back().RotateXAbsolute(90);
	Models.back().ScaleAbsolute(3000);
	Models.back().Update();

	Models.push_back(PrimitiveInst());
	Models.back().CreateInstance(PrimitiveMgr.GetPrimitive(index));
	Models.back().TranslateAbsolute(-3380, 1, 1500);
	Models.back().RotateXAbsolute(90);
	Models.back().ScaleAbsolute(3000);
	Models.back().Update();
	//==========================================================

	textFPS->Create();
	textFPS->TranslateAbsolute(-0.98,0.9,0);
	textFPS->Update();


	index = PrimitiveMgr.CreateModel("Models/CerdoNuevo.X",true);
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
	if (usePhysX)
	{
		if (physxManager.Step(DtTimer.GetDTSecs()))
			for (auto& it : Cubes)
			{
				it->Update();
			}
	}

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
	}
	OnDraw();
}

void TestApp::OnDraw() {
	// ====================== RTs========================
	Tools::PushRT(0);
	pFramework->pVideoDriver->SetCullFace(BaseDriver::BACK);

	for (int i = 0; i < Models.size()-4; i++) {
		Matrix4D temp = Models[i].Scale;
		Models[i].Scale = Models[i].Scale * Scaling(1,-1,1);

		Models[i].Update();
		Models[i].Draw();
		Models[i].Scale = temp;
		Models[i].Update();

	}
	pFramework->pVideoDriver->SetCullFace(BaseDriver::FRONT);
	Tools::PopRT();
	//====================================================
	pFramework->pVideoDriver->Clear();
	

	for (auto& it : Cubes)
	{
		it->Draw();
	}
	for (auto &it : Models)
		it.Draw();

	lightPrimitive.Draw();
	textFPS->Draw();
	dot->Draw();
	sprite->Draw();
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	if (IManager.PressedKey(SDLK_w)) {
		cam.TraslateFront(-40.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_s)) {
		cam.TraslateFront(40.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_d)) {
		cam.TraslateSide(40.0f*DtTimer.GetDTSecs());
	}

	if (IManager.PressedKey(SDLK_a)) {
		cam.TraslateSide(-40.0f*DtTimer.GetDTSecs());
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
	if (IManager.PressedOnceKey(SDLK_l)) {
		usePhysX = !usePhysX;
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