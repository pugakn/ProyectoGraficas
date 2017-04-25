#include "PxCube.h"


using namespace physx;
extern PxPhysics* g_Physics;
extern PxScene *g_scene;
void PxCube::Create(PrimitiveBase* prm, float scale)
{

	//PhysX
	//Material
	PxMaterial* material2 = g_Physics->createMaterial(0.5, 0.5, 0.2);
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
	
	Matrix4D rot{rotQuat.getBasisVector0().x,rotQuat.getBasisVector0().y,rotQuat.getBasisVector0().z,0,
				 rotQuat.getBasisVector1().x,rotQuat.getBasisVector1().y,rotQuat.getBasisVector1().z,0,
				 rotQuat.getBasisVector2().x,rotQuat.getBasisVector2().y,rotQuat.getBasisVector2().z,0,
				 0,0,0,1};
	primitive->m_RotationX = rot;
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
