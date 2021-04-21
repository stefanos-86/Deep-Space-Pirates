#pragma once

//Componets generic types, game objects and managers.
#include "gocVisualModel.h"
#include "gocPhysicsRigidBody.h"
#include "gocVisualModel.h"
#include "gocLogicBase.h"
#include "GameObject.h"
#include "goManager.h"

//Global technical systems
#include "TimedElement.h"
#include "HavokFacade.h"
#include "OgreFacade.h"
#include "SystemClock.h"

/**Collections of functions to avoid common code in the object creators.*/
class CreatorsUtilities
{
public:
	/**Removes the object called name from the havok facade, the ogre facade and the system clock.
	Then deletes it from the go manager. The game logic must be a timed element for this method to be used,
	and all the components related to the said facade must exist.*/
	template <class Logic_t>  //Tried with no template and goLogicBase, but since the inerithance from TimedElement is not at this level
							  //of the hierarchy the casts were not working
	static void coreUnregistration(const std::string& name){
		//Fetch the object in the go manager.
		GameObject* toScrap = goManager::getInstance()->getGameObject(name);
		if (toScrap==0)
		{
			LOG(ERROR) << "Null pointer: go could not return the object called [" << name <<"].";
			throw std::runtime_error("Programming error! Deleting a object not in the goManager!");
		}
		Logic_t* logic = static_cast<Logic_t*>(toScrap->getComponent(GameObjectComponent::GOC_TYPE_LOGIC));
		
		//Disconnect from the clock system.
		SystemClock::getInstance()->deleteAllDurationTimers(logic);
		SystemClock::getInstance()->removeListener(logic);

		//Remove the element from the physical simulation and from the visible world.
		gocPhysicsRigidBody* physics = static_cast<gocPhysicsRigidBody*>(toScrap->getComponent(GameObjectComponent::GOC_TYPE_PHYSICS));
		HavokFacade::getInstance()->deleteEntity(*physics);
		gocVisualModel* visual = static_cast<gocVisualModel*>(toScrap->getComponent(GameObjectComponent::GOC_TYPE_VISUAL));
		OgreFacade::getInstance()->deleteEntity(visual);
	
		//Finally, the dsPirates game object.
		goManager::getInstance()->removeGameObject(*toScrap);
	}

	/**Takes the 3 basic object components, makes a game object out of them, registers them to the goManager and the facades
	 but not to the clock, as the timing requirement may vary (e.g. different duration in timers).*/
	static void coreCreation(const std::string& name, gocLogicBase* logic, gocVisualModel* visual, gocPhysicsRigidBody* physics);
};

