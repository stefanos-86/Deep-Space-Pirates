#pragma once
#include "RadarVisibleInterface.h"


//Return a fixed radar position when scanned.
class FixedRadarTarget : 	public RadarVisibleObject
{
public:
	FixedRadarTarget(const Vector3d& position);

	virtual RadarContact radarScan(const RadarBeam& incomingRadar);

	virtual void receiveSensorId(unsigned int id);

	virtual numeric_id_t getSensorId();

private:
	Vector3d position;
	numeric_id_t id;
};

