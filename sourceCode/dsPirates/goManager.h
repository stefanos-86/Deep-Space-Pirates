#include "stdafx.h"
#pragma once

#include "EasySingleton.h"
#include "GameObject.h"

#include <map>

/** Game Object Manager. It registers and manages game objects.*/
class goManager
{
	// this class is singleton
	SINGLETON(goManager)

public:
	
	/** Type for the game objects data-structure. 
	    A map <name, GameObject*>
	*/
	typedef std::map<GameObject::go_id_type, GameObject*> goTable;

	/** Deletes all game objects and clears the data-structure. */
	void clear();

	/**Returns the game object by name, if it exists.
	@param game object name
	@returns the element corresponding to that name, NULL otherwise. */
	GameObject* getGameObject(GameObject::go_id_type name);

	/** Registers the game object passed as a parameter and returns true if the operation succeded.
	@param game object to register
	@returns true if the operation succeded, false otherwise*/
	bool addGameObject(GameObject const& go);

	/**Remeves the object and all the components.
	If the object is not registered, just return false.
	@param go the object to be dismantled.
	@returns true if the object is actually deleted.*/
	bool removeGameObject(GameObject const& go);
private:

	/** This class is singleton and can't be created (by clients) on the stack */
	goManager() {}

	/** This class is singleton and can't be created (by clients) on the stack */
	~goManager();

	/** This class is singleton and can't be created (by clients) on the heap 
	[More Effective C++, Item 27].*/
	static void* operator new(size_t size) { return ::operator new(size); }
	static void operator delete(void* ptr) { ::operator delete(ptr); }

	/** Game Objects data-structure */
	goTable gameObjects;


};

