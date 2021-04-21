#include "StdAfx.h"
#include "BulletCreator.h"

//Logic
#include "Bullet.h"

//Graphyc
#include "gocVisualModel.h"

//Physics
#include "gocPhysicsRigidBody.h"
#include "ICollisionGeometry.h"
#include "BasicCollisionGeometry.h"

//GOCs
#include "gocVisualModel.h"
#include "gocLogicBase.h"
#include "GameObject.h"
#include "goManager.h"

//Global technical systems
#include "TimedElement.h"
#include "HavokFacade.h"
#include "OgreFacade.h"
#include "SystemClock.h"

#include "CreatorsUtilities.h"

BulletCreator::BulletCreator(void)
{
}


BulletCreator::~BulletCreator(void)
{
}

void BulletCreator::createGunBullet(const Vector3d& initialPosition,
									const dsQuaternion& initialOrietation,
									const Vector3d& velocity,
									const std::string& name,
									const float selfDestroyTime){
	//Create the collision listener/logic.
	Bullet* bulletLogic = new Bullet(name, 100); //Fixed damage power
	gocVisualModel* bulletVisual = new gocVisualModel("Bullet.mesh"); 
	ICollisionGeometry* collisionMesh = new BasicCollisionGeometry(0.05); //A 100 mm gun. Tank stuff!
	gocPhysicsRigidBody* bulletPhysics = new gocPhysicsRigidBody(0, 0, 5, initialPosition, initialOrietation, velocity, collisionMesh, bulletLogic);

	CreatorsUtilities::coreCreation(name, bulletLogic, bulletVisual, bulletPhysics);
	//The time should be the last thing: operation should commence only after registration.
	SystemClock::getInstance()->addListener(bulletLogic);
	SystemClock::getInstance()->addDurationListener(bulletLogic,selfDestroyTime, false);
}

void BulletCreator::dismantleGunBullet(const std::string& name){
	LOG(INFO) << "About to erase " << name;
	CreatorsUtilities::coreUnregistration<Bullet>(name);
	LOG(INFO) << "Erased " << name;
}


