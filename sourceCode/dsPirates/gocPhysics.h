#include "stdafx.h"

#pragma once
#include "gameobjectcomponent.h"

/**Top of the hierarchy for components that define the physic behavior of an object.*/
class gocPhysics : public GameObjectComponent
{
public:
	/**This class is pure abstract.*/
	virtual ~gocPhysics() =0;

	/**Returns the type of the component.
	@returns the object type, the constant string named GOC_TYPE_PHYSICS*/
	virtual const goc_id_type componentID() const {
		return goc_id_type(GameObjectComponent::GOC_TYPE_PHYSICS);
	}

	/**Returns the type of family of the component (the ID of the root of the hierarchy this component belongs to).
	Non-virtual in order to make this class the root of a hierarchy.
	@returns the family type*/
	const goc_id_type familyID() const {
		return goc_id_type(GameObjectComponent::GOC_TYPE_PHYSICS);
	}


};

/**There is nothing to destroy in this class.*/
inline gocPhysics::~gocPhysics() {  }

