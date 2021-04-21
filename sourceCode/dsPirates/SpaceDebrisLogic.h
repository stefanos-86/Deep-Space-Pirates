#pragma once
#include "stdafx.h"
#include "gocCollidableLogic.h"
#include "TimedElement.h"
#include "RadarVisibleInterface.h"
#include "IDGenerator.h"

/**This class is the logic for "dead" objects floating in space (e.g. asteroids, killed spaceships...).
It coordinates the physiscs and the graphics and responds to radar scans.*/
class SpaceDebrisLogic :  public TimedElement, public gocCollidableLogic, public RadarVisibleObject
{
public:
	SpaceDebrisLogic(const std::string& name, int health);
	~SpaceDebrisLogic(void);
	
	/**Takes updates in position/rotation from the physics and updates the graphics.*/
	virtual void timeStep(float time);

	/*Implement the radar interface.*/
	RadarContact radarScan(const RadarBeam& incomingRadar);
	void receiveSensorId(numeric_id_t id);
	numeric_id_t getSensorId();

	/**Override impact handling for testing.*/
	virtual void contactPointCallback( const hkpContactPointEvent& theEvent );

private:
	numeric_id_t sensorId;

	///Health points
	int hp;

};

