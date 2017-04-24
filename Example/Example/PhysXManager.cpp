#include "PhysXManager.h"
using namespace physx;
physx::PxFoundation *g_Foundation;
physx::PxPhysics* g_Physics;
physx::PxScene *g_scene;
physx::PxDefaultCpuDispatcher* g_Dispatcher;
physx::PxCudaContextManager* g_CudaContextManager;
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



	PxCudaContextManagerDesc cudaContextManagerDesc;
	g_CudaContextManager = PxCreateCudaContextManager(*g_Foundation, cudaContextManagerDesc);

	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	g_Dispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = g_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gpuDispatcher = g_CudaContextManager->getGpuDispatcher();

	sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;

	g_scene = g_Physics->createScene(scDesc);
}

bool PhysXManager::Step(float delta)
{
	m_fAccumulator += delta;
	if (m_fAccumulator > STEP_TIME)
	{
		g_scene->simulate(m_fAccumulator);
		g_scene->fetchResults(true);
		m_fAccumulator = 0;
		return true;
	}
	return false;
}
