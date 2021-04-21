#include "StdAfx.h"
#include "WeaponSystem.h"

#include <iostream> //temp for console logging

#include "BulletCreator.h"
#include "dsMath.h"
#include "goManager.h"
#include "gocPhysicsRigidBody.h"

#include "SystemClock.h"

#include "ShipSound.h" //Weapon noises

WeaponSystem::WeaponSystem(void) :
	bulletCreator(new BulletCreator()),
	gunReady(true),
	gunFiring(false),
	machineGunBulletsIdMaker("playerBullets") //Give the name prefix.
{
}


WeaponSystem::~WeaponSystem(void)
{
	delete this->bulletCreator;
}

//Lookup and cache the audio component.
static ShipSound* getNoiseGenerator(){
	static ShipSound* noiseGenerator= 0;
	if (noiseGenerator == 0){
		GameObject* fullSpaceship = goManager::getInstance()->getGameObject("PlayerSpaceship");
		if(fullSpaceship == 0)
			throw std::runtime_error("No game object called PlayerSpaceship - the guns can not make noise!");
		noiseGenerator = (ShipSound*) fullSpaceship->getComponent(GameObjectComponent::GOC_TYPE_AUDIO);
	}
	if(noiseGenerator==0)
		throw std::runtime_error("No audio components in spaceship - the guns can not make noise!");
	return noiseGenerator;
}

void WeaponSystem::openFire(void){

	//Guard to prevent button-smashers to fire faster than the full auto.
	if (! gunReady){
		return;
	}

	
	//Now the trigger is down (gunFiring) and the gun is cycling (gun not ready);
	gunReady=false;
	gunFiring=true;

	//Fire the first round - the timed callback does just this.
	this->timedCallback(0,0);

	//Set up the timer for the following rounds.
	static const float machineGunPeriod = 0.2; //5 rounds per sec

    SystemClock::getInstance()->addDurationListener(this, machineGunPeriod, true);
}

void WeaponSystem::ceaseFire(void){
	//Stop waiting for rounds;
	gunFiring=false;
	//SystemClock::getInstance()->deleteAllDurationTimers(this);
	//add another callback for the gun to cool after the last round.
}

void WeaponSystem::timedCallback(numeric_id_t id, float actualDuration){

	//Use the goManager to "find ourself" and from there the physic component (that we cache).
	static gocPhysicsRigidBody* launchPlatform = 0;
	if (launchPlatform == 0){
		GameObject* launchPointObject = goManager::getInstance()->getGameObject("PlayerSpaceship");
		if(launchPointObject == 0)
			throw std::runtime_error("No game object called PlayerSpaceship - the guns can not work!");
		launchPlatform = (gocPhysicsRigidBody*) launchPointObject->getComponent(GameObjectComponent::GOC_TYPE_PHYSICS);
	}

	//If we are not firing, we don't fire and set that the gun is ready.
	if (! gunFiring){
		SystemClock::getInstance()->deleteAllDurationTimers(this);
		gunReady=true;
		return;
	}  //Else, we create a new bullet and cycle again.

	//Lookup the starting point, direction and velocity.
	Vector3d safetyDistance(7,0,0); //Add some distance in front of the ship to avoid generating bullets inside it.
	Vector3d gunPush(50,0,0); //Part of the initial velocity of the round due to the gun propelling it.
	Vector3d startingPoint = launchPlatform->getPosition() + (launchPlatform->getOrientation() * safetyDistance);
	dsQuaternion gunDirection = launchPlatform->getOrientation();
	Vector3d launchVelocity = launchPlatform->getVelocity() + (launchPlatform->getOrientation() * gunPush); ;

	//Create a bullet in the system.
	static const float bulletSelfDestroyTime = 1;
	this->bulletCreator->createGunBullet(startingPoint,
										 gunDirection,
										 launchVelocity,
										 this->machineGunBulletsIdMaker.getNewStringId(),
										 bulletSelfDestroyTime);
	getNoiseGenerator()->gunShot();
}