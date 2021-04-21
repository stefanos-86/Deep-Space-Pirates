#pragma once

#include <map>
#include "GameObjectComponent.h" //need to know the allowed types

/**This is the base class to represent any object in the game.
It hosts pointers to the various components that implement the functionalities of the object to ease the comunication
between objects.*/
class GameObject
{
public:
	/**type for ID.*/
	typedef std::string go_id_type;

	/**Creates a GameObject given the id.
	@param name A go_id_type that identifies the object.*/
	GameObject(const go_id_type& name) : gameObjectID(name) {}

	/**Virtual for testability reasons.
	
	Virtual destructor, in order to make this class subclassed.
	Any reason other than test classes for this?? A game object is nothing but a component holder!!!*/
	virtual ~GameObject() {this->clearComponents();}
	 
	/**Returns the ID of this GO.*/
	const go_id_type getID() const {
		return gameObjectID;
	}

	/**Changes the ID of this GO.*/
	void setID(const go_id_type& id) {
		gameObjectID = id;
	}

	/**Returns a component of the specified (family) type.
	@param type The type of component that will be returned.
	@return A pointer to a component object, or NULL if the game object has no such component.
	*/
	GameObjectComponent* getComponent(const GameObjectComponent::goc_id_type&  familyID) {
		
		components_table::iterator it= components.find(familyID);

		if ( it !=components.end() )
			return (*it).second;

		return NULL;
	}

	/**Allows to set components at runtime.
	If a GOC of that family already exists, returns the pointer to the replaced GOC.
	@param component Pointer to the component to be added to the object.
	@see GameObjectComponent
	*/
	GameObjectComponent* addComponent(GameObjectComponent* newGOC) {

		// find the component of that family
		components_table::iterator it = components.find(newGOC->familyID());

		// if it exists
		if (it!=components.end()) {
			// current GOC of that family
			GameObjectComponent* tmp = (*it).second;
			// erase the current GOC of that family
			components.erase(it);
			// prepare the new GOC
			newGOC->setOwner(this);
			// insert the new GOC
			components.insert(std::make_pair(newGOC->familyID(),newGOC));
			// return the old GOC
			return tmp;
		}

		// if it does not exist
		// prepare the new GOC
		newGOC->setOwner(this);
		// insert the new GOC
		components.insert(std::make_pair(newGOC->familyID(),newGOC));
		// return NULL, because you didn't replaced anything
		return NULL;
	}

	/**Clears all the components in the table.
	   This deletes the components of this game object.
	*/
	void clearComponents() {
		for (components_table::iterator it=components.begin(); it!=components.end(); ++it) 
			delete (*it).second;
		
		components.clear();
	}

private:

	/**Unique identifier for this object.*/
	go_id_type gameObjectID;

	/**Components are stored here, By (family) type*/
	typedef std::map<GameObjectComponent::goc_id_type, GameObjectComponent*> components_table;
	
	/**Components*/
	components_table components;

	/*Code style. Using a map* in order to avoid to include GameObjectComponent - as I had lot of inclusion issues in the past -
	but keeping a plain string as would be in Java (yes, I believe java does strings right!).
	See also:
	http://www.goingware.com/tips/parameters/headers.html
	http://www.goingware.com/tips/parameters/membervars.html#initialization-list
	*/
};

//syntactic shugar
#define GET_SIBLING(type) this->getOwner()->getComponent(GameObjectComponent::type)