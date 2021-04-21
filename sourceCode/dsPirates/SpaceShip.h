/**@file */
#pragma once
#include "stdafx.h"
#include "gocCollidableLogic.h" //This class can answer to collision notifications.

#include "TimedElement.h"

#include "dsMath.h"


//This struct stores the "public state" of the spaceship (e.g. that can be displayed in the instrument panel).
#include "SpaceShipStatus.h"

//Subsitems that implement the spaceship (the logic takes ownership at construction time).
class Cockpit;
class SpaceShipThrusters;
class SpaceShipControls;
class Radar;
class WeaponSystem;
class FlightControlSystem;

/**This class models the behaviour of a spasceship and exposes the functionalities of this object.
It gives the "control panel" in cockpit of the ship and simulate the spaceship systems.
It is also the "director" for the spaceship (e.g. it has the "main" time step, and calls the physics).

Since this is the spaceship main component it takes ownership of thruestrs, instrument panel and any other sub-component.
It is the OWNER of the objects.
*/
typedef int movementAxis_t;
typedef int direction_t;

class SpaceShip :  public TimedElement, public gocCollidableLogic
{

public:
	SpaceShip(
		const std::string& name,
		SpaceShipThrusters* engine,
		Cockpit* instrumentPanel,
		SpaceShipControls* controls,
		int initialLifePoints,
		Radar* radar,
		WeaponSystem* weapons,
		FlightControlSystem* fcs);
	virtual ~SpaceShip();

	//implements TimedElement
	virtual void timeStep(float time);

	//Implements collisions answer behavior.
	virtual void contactPointCallback( const hkpContactPointEvent& event );


private:

	SpaceShipThrusters* engine;
	Cockpit* instrumentPanel;
	WeaponSystem* weapons;
	SpaceShipControls* controls;
	Radar* radar;
	FlightControlSystem* fcs;

	SpaceShipStatus status;
	
};

