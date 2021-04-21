#include "stdafx.h"
#pragma once

#include "EasySingleton.h"

#include "GameObject.h"

/** Game Object Factory. It provides pseudo-ctors to make game objects. 
    It is possible we'll make use of game object templates and those will be confined here, with no effects on clients.
*/
class goFactory
{
	// this class is singleton
	SINGLETON(goFactory)

public:

	/** Creates a new Game Objects.
	@param name of the game object to create.
	@returns a new game object. */
	GameObject* makeGameObject(GameObject::go_id_type name);

	/** Creates a new SpaceShip object.
	@param name of the spaceship object to create.
	@returns a new spaceship game object.*/
	GameObject* makeSpaceShip(GameObject::go_id_type name);

	// ... other game objects

	// better solution -> using a data-driven (and template) approach as [Game Programming Gems 6, gem 4.6] suggests

private:
	
	/** This class is singleton and can't be created (by clients) on the stack */
	goFactory() {}

	/** This class is singleton and can't be created (by clients) on the stack */
	~goFactory() {}

	/** This class is singleton and can't be created (by clients) on the heap 
	[More Effective C++, Item 27].*/
	static void* operator new(size_t size) { return ::operator new(size);  }
	static void operator delete(void* ptr) { ::operator delete(ptr); }
};

