#include "StdAfx.h"
#include "Cameraman.h"
#include "OgreFacade.h"
#include "goManager.h"
#include "GameObjectComponent.h"
//To attach to objects.
#include "gocVisualModel.h"

Cameraman::Cameraman()
{
	//Require the cameras and the viewport to the facade.
	OgreFacade::getInstance()->newEntity(this);
	
	this->viewport->setBackgroundColour(Ogre::ColourValue(0,0,0)); //this should be irrelevant, as there will be a skybox...

	//adjust the aspect ratio of all cameras; must do it there and not with the rest of the camera setup because it needs the viewport.
	float ratio = float(this->viewport->getActualWidth()) / float(this->viewport->getActualHeight());
	std::map<std::string,Ogre::Camera*>::iterator it;
	for(it=this->cameras.begin(); it!=this->cameras.end(); ++it){
		(it->second)->setAspectRatio(ratio);
	}

	//Ensure we start with the test camera.
	this->doAction("test camera", false);
}

/**Use the game object manager to take the spaceship visual object. Returns null in case of failure.*/
static gocVisualModel* FindPlayerSpaceship(){
	gocVisualModel* ret = 0;
	//Ask to the object manager the player visual object.
	GameObject* playerSpaceship= goManager::getInstance()->getGameObject("PlayerSpaceship"); //Problem! We need to know this global name!
	if(playerSpaceship!=0){
		ret = static_cast<gocVisualModel*>(playerSpaceship->getComponent(GameObjectComponent::GOC_TYPE_VISUAL));
		if (ret==0){
			//DSLOG("The goc of the spaceship could not return the visual component..");
			LOG(WARNING) << "The goc of the spaceship could not return the visual component..";
		}
	} else {
		//DSLOG("The goManager could not return the player spaceship.");
		LOG(ERROR) << "The goManager could not return the player spaceship.";
		throw new std::runtime_error("Someone forgot to instantiate the player spaceship!");
	}
	return ret;
}
	
void Cameraman::addCamera(Ogre::Camera* camera){
	//Set up the thisngs that are common to all cameras. TODO: do we want to make those configurable?
	camera->setFOVy(Ogre::Radian(3.14f/8));
	camera->setFarClipDistance(10000);
	camera->setNearClipDistance(1);
	camera->setAutoTracking (false);
	camera->setFixedYawAxis(false);

	//The position and look at of the cameras change with the name.
	//Cameras are expected not to change too much: hardcoding this values here is ok.
	//TODO: any simple way to get rid of the if chain?
	gocVisualModel* player = FindPlayerSpaceship();
	camera->detachFromParent();
	camera->setPosition(0,0,0);
	if(camera->getName() == "test camera")
	{
		camera->setPosition(20,10,80);
		camera->lookAt(Ogre::Vector3(20,0,0));
	//	camera->setAutoTracking(true, player->getNode());
	} else if(camera->getName() == "look ahead")
	{
		camera->setDirection(1,0,0);
		player->getNode()->attachObject(camera);
	} else if(camera->getName() == "look back")
	{
		camera->setDirection(-1,0,0);
		player->getNode()->attachObject(camera);
	} else if(camera->getName() == "look up")
	{
		camera->setDirection(0,1,0);
		//Fix view up vector.
		Ogre::Vector3 desiredUp(-1,0,0);
		Ogre::Quaternion q =camera->getUp().getRotationTo(desiredUp);
		camera->rotate(q);
		player->getNode()->attachObject(camera);
	} else if(camera->getName() == "follow")
	{
		//The camera starts by pointng to the left. If we then just move it the rotation caused by the lookAt(player position)
		//leaves a horribly tilted camera. To ensure that it rotates nicely we start by pointing it to the front.
		camera->lookAt(1,0,0);
		camera->setPosition(Ogre::Vector3(-20,+20,0));
		player->getNode()->attachObject(camera);
		camera->lookAt(player->getPosition());
	} else if(camera->getName() == "rear-view")
	{
		camera->lookAt(1,0,0); //Same as follow camera... just with a different manouver required by the different direction.
		camera->yaw(Ogre::Radian(Ogre::Degree(180.0)));
		camera->setPosition(Ogre::Vector3(+20,+20,0));
		player->getNode()->attachObject(camera);
		camera->lookAt(player->getPosition());
	}

	this->cameras[camera->getName()]=camera;

}

void Cameraman::assignViewport(Ogre::Viewport* viewport){
	this->viewport=viewport;
}

const std::vector<std::string>& Cameraman::getNames(){
	//hardcoded camera names-same as the actions!
	//Copy in a static to avoid return of locals.
	static std::vector<std::string> names(this->declareActions());

	return names;
}

void Cameraman::doAction(ActionName const& action, bool stop){
	if (stop)  //Act only on key down.
		return;

 //The cameras have the exact same name of the action. This means that we can just use that for the camera lookup.
		std::map<std::string,Ogre::Camera*>::const_iterator camera;
		camera=this->cameras.find(action);
		if(camera == this->cameras.end())		{
			//DSLOG("Somehow we tried to lookup a unknown camera.");
			LOG(WARNING) << "Somehow we tried to lookup a unknown camera.";
		}else {
			this->viewport->setCamera(camera->second);
		}
}


std::vector<ActionName> Cameraman::declareActions(){
	std::vector<ActionName> ret;

	ret.push_back("look ahead");
	ret.push_back("look back");
	ret.push_back("look up");
	ret.push_back("follow");  //Camera behind the player.
	ret.push_back("rear-view"); //Front-to-back
	ret.push_back("test camera");

	return ret;
}
