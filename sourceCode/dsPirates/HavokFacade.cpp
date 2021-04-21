#include "StdAfx.h"
#include "HavokFacade.h"
#include <cstdio>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
// Physics
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
//Visual debugger (available in debug mode only).
#include "VisualDebuggerConnector.h"

/**Re-routes Havok error calls to our logging system.*/
static void HK_CALL errorReport(const char* msg, void* userContext){
	std::string logLine(msg);
	std::cout<<logLine<<std::endl;
	LOG(ERROR) << logLine;
	//TODO: should we do something with or just ignore the user context?
}

SINGLETON_DEF(HavokFacade); 

HavokFacade::HavokFacade()
{
	//Set up Havok memory. 

	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(&havokMalloc, hkMemorySystem::FrameInfo(SOLVER_MEMORY_SIZE) );
	hkBaseSystem::init( memoryRouter, errorReport );

	//Create the world and register it to the system.
	 
	hkpWorldCinfo info;
	info.m_gravity.set( 0,0,0); //Space, no gravity. 
	info.m_simulationType=hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
	info.m_collisionTolerance = 0.1f;; 
	info.setBroadPhaseWorldSize(5000.0f);
	world=new hkpWorld( info );

	//Activate collision system.
	hkpAgentRegisterUtil::registerAllAgents( world->getCollisionDispatcher() );
	world->addWorldPostCollideListener(& this->postCollisionEraser);

	//Visual debugger only in debug mode. Danger! We leave a null pointer if not in debug mode.
	visualDebugger = 0;
	#ifdef _DEBUG
		visualDebugger = new VisualDebuggerConnector(world);
	#endif

}


HavokFacade::~HavokFacade(){
	//Havok shutdown.
	world->removeReference();
	
	#ifdef _DEBUG
		delete visualDebugger;
	#endif

    hkBaseSystem::quit();
    hkMemoryInitUtil::quit();
			
}


///Starts the physics simulation.
void HavokFacade::fireUp(){
}

///Step the physics at each frame.
void HavokFacade::timeStep(float time){
	world->stepDeltaTime(time);
	#ifdef _DEBUG
		visualDebugger->step();
	#endif
}

void HavokFacade::newEntity(const gocPhysicsRigidBody& component){
	  world->addEntity(component.getRigidBody());
}

void HavokFacade::deleteEntity(const gocPhysicsRigidBody& component){
	world->removeEntity(component.getRigidBody());
}

void HavokFacade::requestPostCollisionRemoval(GameItemRemover* remover){
	this->postCollisionEraser.registerForPostCollisionDeletion(remover);
}

#include <Common/Base/keycode.cxx>

// We're using only physics - we undef products even if the keycode is present so
// that we don't get the usual initialization for these products.

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches

#include <Common/Base/Config/hkProductFeatures.cxx>