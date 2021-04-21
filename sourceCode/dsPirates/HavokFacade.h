#pragma once

#include <Common/Base/hkBase.h>  //Always include before the others.
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>

#include "gocPhysicsRigidBody.h"

//Time steps from the system.
#include "TimedElement.h"

//Hanlder for post-collision work (object deletions)
#include "PostCollisionDismantler.h"

class hkpWorld;
class VisualDebuggerConnector;


/**Havok facade: accept the registration of components and commands the engine at the time step.
WORK IN PROGRESS!!!*/

#include "EasySingleton.h"

#define SOLVER_MEMORY_SIZE 16*1024*1024 //16MB for lack of any idea on how much we need.

class HavokFacade : public TimedElement
{
	SINGLETON(HavokFacade);

public:
	HavokFacade();

	///The destructor will also terminate the physics simulation and Havok objects.
	~HavokFacade();

	///Starts the physics simulation.
	void fireUp();

	///Step the physics at each frame.
	void timeStep(float time);

	///Register a physics component.
	void newEntity(const gocPhysicsRigidBody& component);

	///Removes the entity from the simulation.
	void deleteEntity(const gocPhysicsRigidBody& component);

	/**It TAKES OWNERSHIP of the remover to be called once the collision phase is complete.
	The remover then be called to perform the clean-up of any object it is responsible for.
	Then the facade deletes the remover.
	It actually passes the ownership to a inner helper object, but from the client point of view is the same: don't call delete!
	
	TODO: an alternative design is to give access to the helper object and then allow the clients to see the helper object.
	May consider this if the interface of the helper object needs public visibility/requires too much stuff to expose it in the
	facade interface.*/
	void requestPostCollisionRemoval(GameItemRemover* remover);

private:

	///We don't know what this is, but must live until Havok shutdown.
	hkMallocAllocator havokMalloc;
	///Simulated world.
	hkpWorld*		world;

	//Visual debugger for havok; this class owns the object.
	VisualDebuggerConnector* visualDebugger;

	///Removes the objects that had a collision "after the fact" to avoid dangling pointers in collision callbacks.
	PostCollisionDismantler postCollisionEraser;
};

