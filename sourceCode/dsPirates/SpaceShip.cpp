#include "StdAfx.h"
#include "SpaceShip.h"
#include "GameObject.h"
#include "gocLogicBase.h"
#include "gocPhysicsRigidBody.h"
#include "gocVisualModel.h"
#include "SpaceShipThrusters.h"
#include "Cockpit.h"
#include "ShipSound.h"
#include "SpaceShipControls.h"
#include "Radar.h"
#include "WeaponSystem.h"
#include "FlightControlSystem.h"

#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Dynamics/Collide/ContactListener/Util/hkpEndOfStepCallbackUtil.h>

SpaceShip::SpaceShip(const std::string& name,
					SpaceShipThrusters* engine, Cockpit* instrumentPanel, SpaceShipControls* controls,
					int initialLifePoints, Radar* radar, WeaponSystem* weapons, FlightControlSystem* fcs)
	: gocCollidableLogic(name),
	engine(engine),
	weapons(weapons),
	controls(controls),
	instrumentPanel(instrumentPanel),
	radar(radar),
	fcs(fcs)
{ 

	//Prepare the cockpit-displayed (or orherwise shared) data.
	this->status.health = initialLifePoints;
	this->status.maxHealth = initialLifePoints;
	this->status.pitchRate = 0;
	this->status.rollRate = 0;
	this->status.yawRate = 0;
	this->status.linearLocalVelocity = Vector3d::ZERO;
	this->instrumentPanel->setDisplayData(& (this->status));

}


SpaceShip::~SpaceShip(){
	delete this->radar;
	delete this->instrumentPanel;
	delete this->engine;
	delete this->controls;
	delete this->weapons;
	delete this->fcs;
}



void SpaceShip::timeStep(float time){
	//Call the fly by wire to adjust the direction.
	this->fcs->actuate(this->status);

	//TODO either check the actual type or move the time step in the superclass
	gocPhysicsRigidBody* ph = (gocPhysicsRigidBody*) GET_SIBLING(GOC_TYPE_PHYSICS);
	
	ph->timeStep(time);  //the step os hardcoded.
	
	//now we can tell the other components about the changes
	//TODO: test for the actual type
	gocVisualModel* v = (gocVisualModel*) this->getOwner()->getComponent(GameObjectComponent::GOC_TYPE_VISUAL);
	
	Vector3d newPosition=ph->getPosition();
	v->setPosition(newPosition);

	dsQuaternion newOrientation = ph->getOrientation();
	v->setDirection(newOrientation);

	//Update the shared data.
	ph->getAngularVelocities(this->status.pitchRate, this->status.rollRate, this->status.yawRate);
	this->status.linearLocalVelocity = ph->getLocalVelocity();
	
	//Refresh the radar.
	this->instrumentPanel->cleanRadar();
	this->radar->scan();
};

void SpaceShip::contactPointCallback( const hkpContactPointEvent& theEvent ){
	LOG(INFO) << "Spaceship impact";
	if(theEvent.m_firstCallbackForFullManifold){
		static const float impactFactorConstant = 0.5;
		int impactDamage = (int)( theEvent.getSeparatingVelocity() * theEvent.getSeparatingVelocity() * impactFactorConstant);
		if (impactDamage > this->status.health) 
			this->status.health = 0;
		else
			this->status.health -= impactDamage;

		//Quasi-random impact damage on components (things that may break)
		float failureLevel = (float)rand()/(float)RAND_MAX;
		if(impactDamage > 0.5*(failureLevel * 100))  //May have to tweak the probability.
			this->instrumentPanel->setBroken(); //There is only this component, by now, so...

		ShipSound* soundEffects = (ShipSound*) GET_SIBLING(GOC_TYPE_AUDIO);	
		soundEffects->collisionSound();

		//Too many broken things/critical situation: play alarm.
		//if (some condition to be decided){
			soundEffects->startMasterAlarm();
			this->instrumentPanel->masterAlarmOn();
		//}

		//TODO: gun hit damage - since by now it is impossible to get shot, this can wait.

		//...and if the life goes to 0 or less... game over! (TODO!)
	}
	//Else, at least some extra 9 calls that I can not explain!
	//Need to study collision filtering. TODO!
}
