#include "StdAfx.h"
#include "SpaceDebrisLogic.h"

#include "GameObject.h"
#include "gocPhysicsRigidBody.h"
#include "gocVisualModel.h"

#include "Bullet.h" //Special collision handling.

//Destruction on damage
#include "SpaceDebrisCreator.h" 
#include "DebrisPostCollisionRemover.h"
#include "HavokFacade.h"

#include "goManager.h"

//Visual effects on collision
#include "OgreFacade.h"
#include "EffectsLibrary.h"

SpaceDebrisLogic::SpaceDebrisLogic(const std::string& name, int health)
	: gocCollidableLogic(name),
	hp(health)
{}

SpaceDebrisLogic::~SpaceDebrisLogic()
{}

//By now this is very, very similar to the spaceship logic (that we expect to become more complex later on - this is not to
//be seen as duplicated code).
void SpaceDebrisLogic::timeStep(float stepDuration)
{
	gocPhysicsRigidBody* ph = (gocPhysicsRigidBody*) GET_SIBLING(GOC_TYPE_PHYSICS);
	ph->timeStep(stepDuration);
	gocVisualModel* v = (gocVisualModel*) this->getOwner()->getComponent(GameObjectComponent::GOC_TYPE_VISUAL);
	
	Vector3d newPosition=ph->getPosition();
	v->setPosition(newPosition);

	dsQuaternion newOrientation = ph->getOrientation();
	v->setDirection(newOrientation);
}


RadarVisibleObject::RadarContact SpaceDebrisLogic::radarScan(const RadarBeam& incomingRadar){
	//return the physics position
	gocPhysicsRigidBody* ph = (gocPhysicsRigidBody*) GET_SIBLING(GOC_TYPE_PHYSICS);
	RadarVisibleObject::RadarContact ret;
	ret.position=ph->getPosition();
	return ret;
}

void SpaceDebrisLogic::receiveSensorId(numeric_id_t id){
	this->sensorId = id;
}

numeric_id_t SpaceDebrisLogic::getSensorId(){
	return this->sensorId;
}

void SpaceDebrisLogic::contactPointCallback( const hkpContactPointEvent& theEvent ){
		//This object is not damaged by collision - no "ramming" on it.

		//Special damage if colliding with a bullet. First of all find it.
		gocCollidableLogic* object0 = (gocCollidableLogic*) theEvent.getBody(0)->getUserData();
		gocCollidableLogic* object1 = (gocCollidableLogic*) theEvent.getBody(1)->getUserData();
		//find "the bullet"
		Bullet* bullet = 0;
		if (object0 != 0 && dynamic_cast<Bullet*>(object0))
			bullet = dynamic_cast<Bullet*>(object0);
		if (object1 != 0 && dynamic_cast<Bullet*>(object1))
			bullet = dynamic_cast<Bullet*>(object1);
		if (bullet!=0){
			GameObject* test = goManager::getInstance()->getGameObject(bullet->getObjectName());
			if (!test)
				throw std::runtime_error("Programming error! Access to a object not in the goMgr!");
			
			//Valid bullet found: discard the damage, destroy the object if needed.
			this->hp -= bullet->getWeaponDamage();
			if (this->hp <= 0){
				DebrisPostCollisionRemover* pcr = new DebrisPostCollisionRemover(this->getObjectName());
				HavokFacade::getInstance()->requestPostCollisionRemoval(pcr);
			}

		}

		//Visual effects: little cloud of smoke.
		hkVector4 impactPointHK = theEvent.m_contactPoint->getPosition();
		Vector3d impactPoint(impactPointHK(0), impactPointHK(1), impactPointHK(2)); 
		OgreFacade::getInstance()->activateEffect("Grey smoke puff", impactPoint);
}