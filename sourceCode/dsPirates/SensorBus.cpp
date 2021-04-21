#include "StdAfx.h"
#include "SensorBus.h"

#include "RadarVisibleInterface.h"

numeric_id_t SensorBus::registerTarget(RadarVisibleObject* target){
	numeric_id_t id=this->idMaker.getNewNumericId();
	target->receiveSensorId(id);
	this->radarTargets.push_back(target);
	return id;
}

	/**Remove the object from the publishing list.*/
void SensorBus::unSubscribe(numeric_id_t id){
	//Search and remove the object.
	std::vector<RadarVisibleObject*>::iterator it=this->radarTargets.begin();
	while(it!=this->radarTargets.end() && (*it)->getSensorId() != id)
		++it;
	if(it!=this->radarTargets.end())
		this->radarTargets.erase(it); 
}

	/**Scan all the available contacts. Fills the vector of returned stuff.
	In the future may filter by direction, coalesce contacts too close to be resolved...*/
void SensorBus::scan(const RadarVisibleObject::RadarBeam& emitter, std::vector<RadarVisibleObject::RadarContact>& detectedContacts){
	//The emitter is ignored by now.

	//Scan all the contacts.
	std::vector<RadarVisibleObject*>::const_iterator it;
	for(it=this->radarTargets.begin(); it!=this->radarTargets.end(); ++it){
		detectedContacts.push_back((*it)->radarScan(emitter));
	}
	//The list is not "refined" by now.
}