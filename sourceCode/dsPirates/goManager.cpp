#include "StdAfx.h"
#include "goManager.h"

SINGLETON_DEF(goManager)

goManager::~goManager()
{
	// just clear the map
	clear();
}


void goManager::clear() {
	// first, delete all pointers
	for (goTable::iterator it = gameObjects.begin(); it!=gameObjects.end(); ++it){
		delete it->second; 
	}
	
	// then, clear the map
	gameObjects.clear();
}

GameObject* goManager::getGameObject(GameObject::go_id_type name) {
	LOG(INFO) << "Query for go " <<name;
	// does an element with that name exist?
	goTable::iterator it = gameObjects.find(name);

	// if it does, return it!
	if (it != gameObjects.end() )
		return it->second;

	// return NULL, otherwise
	return NULL;

}

bool goManager::addGameObject(GameObject const& go) {
	LOG(INFO) << "Creation of go " << go.getID();

	// if go's name is already registered, just say no and return false!
	if (gameObjects.count(go.getID()) > 0)
		return false;

	// otherwise, register a new game object
	GameObject* nonConstGo = const_cast<GameObject*>( &go );
	gameObjects.insert(std::make_pair(go.getID(), nonConstGo));

	return true;
}


bool goManager::removeGameObject(GameObject const& go){
	LOG(INFO) << "Removal of go " << go.getID();
	goTable::iterator toBeDeleted = gameObjects.find(go.getID());
	if (toBeDeleted==gameObjects.end())
		return false; //Should never happen, but...
	delete toBeDeleted->second;
	gameObjects.erase(toBeDeleted);
	return true;

}