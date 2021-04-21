#pragma once

#include "RadarVisibleInterface.h"

#include "gocLogic.h"
#include "IDGenerator.h"

#include <vector>

/**This is the publisher for the radar system. It knows all the "scannable" objects and can propagate
the sensor scans.
It is a game object, so that other "pieces" can find it when needed.*/
class SensorBus : public gocLogic
{
public:

	/**Register a listener and returns a identifier for it.
	The identifier can be used to un-subscribe the object.*/
	numeric_id_t registerTarget(RadarVisibleObject* target);

	/**Remove the object from the publishing list.*/
	void unSubscribe(numeric_id_t id);

	/**Scan all the available contacts. Fills the vector of returned stuff.
	In the future may filter by direction, coalesce contacts too close to be resolved...
	(this could be implemented in the radar too, but it make sense to centralize the sensor processing (?)).*/
	void scan(const RadarVisibleObject::RadarBeam& emitter, std::vector<RadarVisibleObject::RadarContact>& detectedContacts);

private:
	///List of radar targets. NOT OWNED by this class.
	std::vector<RadarVisibleObject*> radarTargets;

	///Helper to give IDs to radar contacts.
	IDGenerator idMaker;
};

