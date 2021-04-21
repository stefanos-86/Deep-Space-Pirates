#include "StdAfx.h"
#include "FixedRadarTarget.h"

#include "RadarVisibleInterface.h"

FixedRadarTarget::FixedRadarTarget(const Vector3d& position)
{
	this->position = position;
}

RadarVisibleObject::RadarContact FixedRadarTarget::radarScan(const RadarBeam& incomingRadar){
	RadarVisibleObject::RadarContact ret;
	ret.position = this->position;
	return ret;
}

void FixedRadarTarget::receiveSensorId(unsigned int id){
	this->id = id;
}

numeric_id_t FixedRadarTarget::getSensorId(){
	return this->id;
}