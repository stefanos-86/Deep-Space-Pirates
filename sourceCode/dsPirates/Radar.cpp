#include "StdAfx.h"
#include "Radar.h"

//Vectors and coordinates conversions
#include "dsMath.h"
#include "gocVisualModel.h"

//Object search
#include "goManager.h"
#include "SensorBus.h"

//Prints contact on this object.
#include "Cockpit.h"

//lists of targets
#include <vector>

Radar::Radar(Cockpit* screen, double range, gocVisualModel* forCoordinates) :
	screen(screen),
	squaredRange(range*range),
	coordinates(forCoordinates)
{
}


Radar::~Radar(void)
{
}


//Assume it returns the position of every target in WC!
static std::vector<Vector3d> GetTargets(){
	//TODO: find a way to query for other "radar-visible" game objects.

	//Return fake test contacts. Assume spaceship in the origin.
	std::vector<Vector3d> ret;
	//ret.push_back(Vector3d(10, 0, 0)); //Front

	GameObject* gameGlobalRadar = goManager::getInstance()->getGameObject("SensorBus");
	if(gameGlobalRadar == 0)
		throw std::runtime_error("No game object called SensorBus - the radar can not work!");

	SensorBus* sb = (SensorBus*) gameGlobalRadar->getComponent(GameObjectComponent::GOC_TYPE_LOGIC);
	std::vector<RadarVisibleObject::RadarContact> contacts;
	RadarVisibleObject::RadarBeam beam;
	sb->scan(beam,contacts);
	
	std::vector<RadarVisibleObject::RadarContact>::const_iterator it;
	for (it=contacts.begin(); it!=contacts.end(); ++it)
		ret.push_back(it->position);
		
	return ret;
}

void Radar::scan(){
	std::vector<Vector3d> targets = GetTargets();

	std::vector<Vector3d>::const_iterator it;
	for (it=targets.begin(); it!=targets.end(); ++it){
		//convert position in local coords.
		Vector3d contact = this->coordinates->convertInMyLocal(*it);

		//Check if the target is inside the range and in front of the radar
		if(contact.squaredLength() < this->squaredRange)
		{
			//Compute the direction of the target.
			contact.normalise();
			//Projects on the ZY plane/sends the vertical/lateral displacement to the radar.
			this->screen->addRadarContact(contact.z, contact.y, contact.x > 0);
		}
	}
}

