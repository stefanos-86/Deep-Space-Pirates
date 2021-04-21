#include "StdAfx.h"
#include "Bullet.h"

#include "GameObject.h"
#include "gocPhysicsRigidBody.h"
#include "gocVisualModel.h"


//This class can answer to collision notifications (inflict additional damage and self-destroy).
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>

//To dismantle the object.
#include "BulletCreator.h"
#include "BulletPostCollisionremover.h"
#include "HavokFacade.h"

Bullet::Bullet(const std::string& name, int power) :
	gocCollidableLogic(name),
	power(power)
{
}


Bullet::~Bullet(void)
{
}

void Bullet::timeStep(float stepDuration){
	//Copy the physics data in the visual for display.
	gocPhysicsRigidBody* ph = (gocPhysicsRigidBody*) GET_SIBLING(GOC_TYPE_PHYSICS);
	ph->timeStep(stepDuration);
	gocVisualModel* v = (gocVisualModel*) this->getOwner()->getComponent(GameObjectComponent::GOC_TYPE_VISUAL);
	
	Vector3d newPosition=ph->getPosition();
	v->setPosition(newPosition);

	dsQuaternion newOrientation = ph->getOrientation();
	v->setDirection(newOrientation);
}

void Bullet::timedCallback(numeric_id_t id, float actualDuration){
	LOG(INFO) << "Bullet end of life by timer " << this->getObjectName();
	BulletCreator destroyer;
	destroyer.dismantleGunBullet(this->getObjectName());
}

void Bullet::contactPointCallback( const hkpContactPointEvent& event ){
	//Self-remove from the game after the collision phase to be sure that the target hit by
	//the bullet has a valid pointer to it.
	LOG(INFO) << "Bullet end of life by impact " << this->getObjectName();
	
	BulletPostCollisionremover* pcr = new BulletPostCollisionremover(this->getObjectName());
	HavokFacade::getInstance()->requestPostCollisionRemoval(pcr);	
}

int Bullet::getWeaponDamage(){
	return this->power;
}

