#include "StdAfx.h"
#include "goFactory.h"


GameObject* goFactory::makeGameObject(GameObject::go_id_type name) {
	return new GameObject(name);
}

GameObject* goFactory::makeSpaceShip(GameObject::go_id_type name) {
	return new GameObject(name); // soon available...
}