#include "stdafx.h"
#pragma once

class GameObject;

/**Superclass for the game object components, that is the object where the functionalities of the object are implemented.*/
class GameObjectComponent
{
public:

	/**type for ID.*/
	typedef std::string goc_id_type;

	/**Object components types: remember that they are also used for component families.*/
	static const goc_id_type GOC_TYPE_VISUAL; //can only declare, definitions are in the CPP file
	static const goc_id_type GOC_TYPE_PHYSICS;
	static const goc_id_type GOC_TYPE_LOGIC;
	static const goc_id_type GOC_TYPE_AUDIO;
	static const goc_id_type GOC_TYPE_TEST_AND_DEBUG; //not for use in the game!

	/**Creates a component.*/
	GameObjectComponent() : owner(NULL) {}

	/** (Pure) Virtual destructor, in order to make this class pure abstract.*/
	virtual ~GameObjectComponent() = 0;

	/**Returns the type of the component.
	@returns the object type*/
	virtual const goc_id_type componentID() const = 0;

	/**Returns the type of family of the component (the ID of the root of the hierarchy this component belongs to).
	@returns the family type*/
	virtual const goc_id_type familyID() const = 0;

	/**Sets the owner (GameObject) of this components.
	@param go A pointer to the Game object that is using this component. NULL if the component has not be assigned yet.
	@see GameObject*/
	void setOwner(GameObject* go) {
		owner = go;
	}

	/**Returns the owner of this component.
	@returns the GameObject that owns this component*/
	GameObject* getOwner() const {
		return owner;
	}

private : 

	/**The component is assignedd to the object pointed here.*/
	GameObject* owner;
};

inline GameObjectComponent::~GameObjectComponent() { }

