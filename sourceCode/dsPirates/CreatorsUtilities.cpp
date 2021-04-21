#include "StdAfx.h"
#include "CreatorsUtilities.h"


void CreatorsUtilities::coreCreation(const std::string& name, gocLogicBase* logic, gocVisualModel* visual, gocPhysicsRigidBody* physics){
	GameObject* object=new GameObject(name);
	object->addComponent(visual);
	object->addComponent(physics);
	object->addComponent(logic);
	goManager::getInstance()->addGameObject(*object);

	HavokFacade::getInstance()->newEntity(*physics);
	OgreFacade::getInstance()->newEntity(visual);
}