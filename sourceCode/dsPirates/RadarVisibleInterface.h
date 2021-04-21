#pragma once

#include "dsMath.h"
#include "IDGenerator.h"
#include <vector>

/**Foundation to simulate the radar system.
Every target must implement the interface and be able to be scanned.
It receives data about what is "painting" it, and must return data about itself (or nothing, garbage... depends on counter measures).

Such object should be regitered with the RadarScanner publisher.

Many possibilities of future evolution are in the comments. Could be easily extended to any kind of "sensor".
*/

class RadarVisibleObject{
public:
	typedef struct {
		//Not used for now.
		//Vector3d positionOfEmitter;
		//SomeData volumeOfSpaceScanned
		//SomeType emitterType;
		//double signalStrength;
		//...
	} RadarBeam;


//The contact is not expected to tell the truth. Counter-measures and the like.
	typedef struct {
		Vector3d position; //Position of the object.
		//Vector3d velocity;
		//SomeType contactType;
		//SomeType team; //Friend or foe
		//double signalStrength; //Lower for smaller targets...
	} RadarContact;

	/**Each object is detected by the radar when it answers this call.
	The object if free to react in any way it likes (store the "scan" for passive radar detection,
	scramble the results as with ECM...).
	@incomingRadar information about the radar that is scanning the object
	@return the result of the radar "pass" on the object.
	TODO: pass a vector where the object can store more than 1 contact, if the need be (e.g. ECM).
	Something like  radarScan(const RadarBeam& incomingRadar, std::vector<RadarContact>& contacts) )*/
	virtual RadarContact radarScan(const RadarBeam& incomingRadar) = 0;

	/**Allows the sensor bus to manage this object.*/
	virtual void receiveSensorId(numeric_id_t id) = 0;

	/**Allows the sensor bus to manage this object.*/
	virtual numeric_id_t getSensorId() = 0;
};