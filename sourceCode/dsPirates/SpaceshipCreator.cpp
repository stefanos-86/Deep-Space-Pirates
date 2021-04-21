#include "StdAfx.h"
#include "SpaceshipCreator.h"

//Logic and varius pieces of the spaceship.
#include "SpaceShip.h"
#include "SpaceShipControls.h"
#include "SpaceShipThrusters.h"
#include "Cockpit.h"
#include "Radar.h"
#include "WeaponSystem.h"
#include "FlightControlSystem.h"

//Graphyc
#include "gocVisualModel.h"

//Sound effects.
#include "ShipSound.h"

//Physics
#include "gocPhysicsRigidBody.h"
#include "ICollisionGeometry.h"
#include "CustomCollisionGeometry.h"

//GOCs
#include "gocVisualModel.h"
#include "gocLogicBase.h"
#include "GameObject.h"
#include "goManager.h"

//Global systems
#include "TimedElement.h"
#include "HavokFacade.h"
#include "OgreFacade.h"
#include "SystemClock.h"
#include "InputMapper.h"

void SpaceshipCreator::createPlayerSpaceship(InputMapper& inputMapper){
	gocVisualModel* playerVisual = new gocVisualModel("Ship1.mesh"); 

	//The logic here is divided in several classes.
	SpaceShipThrusters* engine = new SpaceShipThrusters(10000.0f, 10000.0f);
	Cockpit* instruments = new Cockpit(*OgreFacade::getInstance()); 
	WeaponSystem* weapons = new WeaponSystem();
	FlightControlSystem* governor = new FlightControlSystem(*engine, 10, 0.5);
	SpaceShipControls* spc = new SpaceShipControls(engine, instruments, weapons, governor);
	Radar* radar = new Radar(instruments, 2000, playerVisual);
	SpaceShip* playerLogic = new SpaceShip("Player", engine, instruments, spc, 200, radar, weapons, governor);
	
//BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!
// THIS IS CATASTROPHICAL! The path in NOT THE SAME in debug and release! Must fix (e.g. start using the same paths...
	ICollisionGeometry* collisionMesh = CustomCollisionGeometry::loadFromFile("../../media/dsp/physics/SHIP001.obj");
//TODO: collisionMesh = Null if file not found -> add a check somewhere.

	gocPhysicsRigidBody* playerSimulation = new gocPhysicsRigidBody(engine, engine, 5000, Vector3d::ZERO, dsQuaternion::IDENTITY, Vector3d::ZERO, collisionMesh, playerLogic); //BAD?? The phisics has a direct pointer to the logic. It should (?) use the components sistem.

	ShipSound* soundFx = new ShipSound();

	GameObject* playerShip=new GameObject("PlayerSpaceship");//The name stays fixed to not have to track down all the pieces...
														  //Is used in the cameraman. We may want to make this a game wide constant.
	playerShip->addComponent(playerVisual);
	playerShip->addComponent(playerLogic);
	playerShip->addComponent(playerSimulation);
	playerShip->addComponent(soundFx);
	goManager::getInstance()->addGameObject(*playerShip);

	SystemClock::getInstance()->addListener(playerLogic); 
	SystemClock::getInstance()->addListener(instruments);
	static const float masterAlarmSemiPeriod = 0.5;
	SystemClock::getInstance()->addDurationListener(instruments, masterAlarmSemiPeriod, true);

	OgreFacade::getInstance()->newEntity(playerVisual);
	HavokFacade::getInstance()->newEntity(*playerSimulation);

	inputMapper.registerActionsProvider(spc);
}
