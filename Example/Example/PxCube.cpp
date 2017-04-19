#include "PxCube.h"


using namespace physx;
extern PxPhysics* g_Physics;
extern PxScene *g_scene;
void PxCube::Create(PrimitiveBase* prm, float scale)
{

	//PhysX
	//Material
	PxMaterial* material2 = g_Physics->createMaterial(0.2, 0.2, 0.2);
	//Box
	PxTransform boxPos(PxVec3(0, 0, 0));
	PxBoxGeometry boxGeo(PxVec3(scale, scale, scale));
	body = PxCreateDynamic(*g_Physics, boxPos, boxGeo, *material2, 1.0f);


	g_scene->addActor(*body);
	primitive = new PrimitiveInst();
	primitive->CreateInstance(prm);
	primitive->ScaleAbsolute(scale);
	primitive->Update();
}

void PxCube::Update()
{
	PxVec3 pos = body->getGlobalPose().p;
	PxQuat rotQuat = body->getGlobalPose().q;
	float rotX,rotY,rotZ;
	rotQuat.toRadiansAndUnitAxis(rotX, PxVec3(1, 0, 0));
	rotQuat.toRadiansAndUnitAxis(rotY, PxVec3(0, 1, 0));
	rotQuat.toRadiansAndUnitAxis(rotZ, PxVec3(0, 0, 1));
	primitive->RotateXAbsolute(rotX * 57.2958f);
	primitive->RotateYAbsolute(rotY * 57.2958f);
	primitive->RotateZAbsolute(rotZ * 57.2958f);
	primitive->TranslateAbsolute(pos.x,pos.y,pos.z);
	primitive->Update();
}

void PxCube::Draw()
{
	primitive->Draw();
}

void PxCube::Destroy()
{
	delete primitive;
	body->release();
}

void PxCube::Traslate(Vector3D trsl)
{
	body->setGlobalPose(PxTransform(trsl.x,trsl.y,trsl.z));
}

PxCube::PxCube()
{
}


PxCube::~PxCube()
{
}
