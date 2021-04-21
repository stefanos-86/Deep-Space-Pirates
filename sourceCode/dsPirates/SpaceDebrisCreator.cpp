#include "StdAfx.h"
#include "SpaceDebrisCreator.h"

//Logic
#include "SpaceDebrisLogic.h"

//Graphyc
#include "gocVisualModel.h"

//Physics
#include "gocPhysicsRigidBody.h"
#include "ICollisionGeometry.h"
#include "BasicCollisionGeometry.h"

//GOCs
#include "gocVisualModel.h"
#include "gocLogicBase.h"
#include "GameObject.h"
#include "goManager.h"

//Global technical systems
#include "TimedElement.h"
#include "HavokFacade.h"
#include "OgreFacade.h"
#include "SystemClock.h"

//Global in-game systems.
#include "SensorBus.h"

#include "CreatorsUtilities.h"

//Lookup and return the sensor bus in the the goManager
SensorBus* grabSensorBus(){
	GameObject* gameGlobalRadar = goManager::getInstance()->getGameObject("SensorBus");
	if(gameGlobalRadar == 0)
		throw std::runtime_error("No game object called SensorBus - can not register a radar target!");
	SensorBus* sb = (SensorBus*) gameGlobalRadar->getComponent(GameObjectComponent::GOC_TYPE_LOGIC);
	return sb;
}

void SpaceDebrisCreator::createCollisionTarget(std::string name, double x, double y, double z){
	gocVisualModel* crashTargetVisual = new gocVisualModel("crashTarget.mesh"); 
	SpaceDebrisLogic* crashTargetLogic = new SpaceDebrisLogic(name, 200); //Hardcoded HPs.
	ICollisionGeometry* collisionMesh = new BasicCollisionGeometry(2);
	gocPhysicsRigidBody* crashTargetPhysiscs = new gocPhysicsRigidBody(0, 0, 500, Vector3d(x,y,z), dsQuaternion::IDENTITY, Vector3d::ZERO, collisionMesh, crashTargetLogic);

	CreatorsUtilities::coreCreation(name, crashTargetLogic, crashTargetVisual, crashTargetPhysiscs);

	//Object fully built. Register to "non-core" subsystems.
	SensorBus* sb = grabSensorBus();
	sb->registerTarget(crashTargetLogic);

	SystemClock::getInstance()->addListener(crashTargetLogic);
}

void SpaceDebrisCreator::dismantleCollisionTarget(const std::string& name){
	//Removal from the sensor bus.
	SensorBus* sb = grabSensorBus();
	//Take the sensor bus ID.
	GameObject* target = goManager::getInstance()->getGameObject(name);
	if (target!=0){ //The object may have already been destroyed - "dangling names".
		SpaceDebrisLogic* logic = static_cast<SpaceDebrisLogic*>(target->getComponent(GameObjectComponent::GOC_TYPE_LOGIC));
		sb->unSubscribe(logic->getSensorId());

		//Removal from all the rest.
		LOG(INFO) << "About to erase " << name;
		CreatorsUtilities::coreUnregistration<SpaceDebrisLogic>(name);
	}
}

void SpaceDebrisCreator::dismantleCollisionTarget(const std::vector<std::string>& names){
	std::vector<std::string>::const_iterator name;
	for (name=names.begin(); name!=names.end(); ++name){
		this->dismantleCollisionTarget(*name);
	}
}

void SpaceDebrisCreator::createReferenceAxes(){
	gocVisualModel* axesVisual = new gocVisualModel("Axes.mesh");
	gocLogic* axesLogic = new gocLogicBase("Axes");  //Every object that goes in the gocManager must have a name, even if there is no other logic.
	GameObject* axes = new GameObject("Cartesian axes");
	axes->addComponent(axesVisual);
	axes->addComponent(axesLogic);
	goManager::getInstance()->addGameObject(*axes);

	OgreFacade::getInstance()->newEntity(axesVisual);
}

void SpaceDebrisCreator::createReferenceCompass(){
	gocVisualModel* compassVisual = new gocVisualModel("Compass.mesh"); 
	gocLogic* compassLogic = new gocLogicBase("Reference points.");

	GameObject* compass = new GameObject("Orientation compass"); 
	compass->addComponent(compassVisual);
	compass->addComponent(compassLogic);
	goManager::getInstance()->addGameObject(*compass);

	OgreFacade::getInstance()->newEntity(compassVisual);
}


