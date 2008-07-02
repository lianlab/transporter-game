#include "transporter.h"

PhysicsScene::PhysicsScene()
{
	physicsWorld = NULL;
	game = NULL;
	isSceneInited = false;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

PhysicsScene::~PhysicsScene()
{
	cleanUp();
	if(physicsWorld)
	{
		//delete physicsWorld;
		physicsWorld = NULL;
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

hkpWorld* PhysicsScene::getWorld()
{
	return physicsWorld;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

bit PhysicsScene::init( Game* game )
{
	if(isSceneInited)
	{
		return true;
	}

	this->game = game;

	hkpWorldCinfo info;
	info.m_collisionTolerance = 0.1f;
	info.m_gravity.set(0.0f, -9.8f, 0.0f);
	info.setBroadPhaseWorldSize(2500.0f) ;
	info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);

	physicsWorld = new hkpWorld(info);

	hkpAgentRegisterUtil::registerAllAgents(physicsWorld->getCollisionDispatcher());

	isSceneInited = true;

	return true;
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void PhysicsScene::update(u32 timeStepMs)
{
	UpdateListenerMgr::getInstance()->update(EV_UPDATE_PHYSICS_FORCE,timeStepMs);
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void PhysicsScene::cleanUp()
{
	//TODO: clear all entities
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void PhysicsScene::lock()
{
	if(physicsWorld)
	{
		physicsWorld->lock();
	}
}

//覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧

void PhysicsScene::unlock()
{
	if(physicsWorld)
	{
		physicsWorld->unlock();
	}
}