#include "stdafx.h"

#pragma once
#include "gameobjectcomponent.h"


class gocVisual :
	public GameObjectComponent
{

public:
	
	/** (Pure) Virtual destructor, in order to make this class pure abstract.*/
	virtual ~gocVisual() = 0;

	/**Returns the type of the component.
	@returns the object type*/
	virtual const goc_id_type componentID() const {
		return goc_id_type(GameObjectComponent::GOC_TYPE_VISUAL);
	}

	/**Returns the type of family of the component (the ID of the root of the hierarchy this component belongs to).
	Non-virtual in order to make this class the root of a hierarchy.
	@returns the family type*/
	const goc_id_type familyID() const {
		return goc_id_type(GameObjectComponent::GOC_TYPE_VISUAL);
	}

};

inline gocVisual::~gocVisual() {  }

