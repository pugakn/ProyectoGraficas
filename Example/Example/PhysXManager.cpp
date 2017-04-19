#include "PhysXManager.h"
using namespace physx;
physx::PxFoundation *g_Foundation;
physx::PxPhysics* g_Physics;
physx::PxScene *g_scene;
void PhysXManager::Init()
{
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	g_Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!g_Foundation)
		exit(-1);

	bool recordMemoryAllocations = true;
	//auto mPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);


	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_Foundation,
		PxTolerancesScale(), recordMemoryAllocations);
	if (!g_Physics)
		exit(-1);

	PxSceneDesc scDesc(g_Physics->getTolerancesScale());
	scDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	scDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	scDesc.filterShader = PxDefaultSimulationFilterShader;

	g_scene = g_Physics->createScene(scDesc);
}

void PhysXManager::Step(float delta)
{
	m_fAccumulator += delta;
	if (!m_fAccumulator < STEP_TIME)
	{
		m_fAccumulator -= STEP_TIME;
		g_scene->simulate(STEP_TIME);
		g_scene->fetchResults(true);
	}
}
