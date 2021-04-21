#include "stdafx.h"
#include "OgreFacade.h"
#include "Ogre.h"
#include "ConfigurationStore.h"
#include "gocVisualModel.h"
#include "gocVisualCamera.h"
#include "GameObject.h"
#include "SystemClock.h"
#include <exception>
#include <string>

//To find the system name of a object.
#include "gocLogicBase.h"

#include "EffectsLibrary.h" //Particle systems.

using namespace std;
using namespace Ogre;
using namespace dsc;

SINGLETON_DEF(OgreFacade); 

OgreFacade::OgreFacade() : ready(false){

}

OgreFacade::~OgreFacade()
{
	//If you get exception on the next line after working on overlays, you can cover up with: OverlayManager::getSingleton().destroyAll(); (but is better if you fix the bug).
	delete this->ogreRoot;
	this->ogreRoot=0;
	LOG(INFO) << "Ogre root deleted";
	delete this->effects;
	this->effects=0;
}

/**Set up the ogre system.
This is a singleton: the default constructor does not have parameters (the EasySingleton macroes are not so flexible).
It is MANDATORY to call this method after creating the instance.
It SHOULD stop automatically if called twice*/
void OgreFacade::init(InputMapper* inputSubSystem){
	static bool alreadyExecuted=false; //the ready class variable may be accessed elsewere...

	//facade already initialized? Check BOTH ways, just in case...
	assert(this->ready == false && alreadyExecuted == false);

	LOG(INFO) << "Saving parameters";
	this->input=inputSubSystem;

	LOG(INFO) << "Setting up ogre";
	Ogre::String lPluginsFileName = CONFIGURATION(DS_OGRE_PLUGINS_FILE_NAME);
	Ogre::String lConfigFileName =  CONFIGURATION(DS_OGRE_CONFIG_FILE_NAME);
	Ogre::String lLogFileName = CONFIGURATION(DS_OGRE_LOG_FILE_NAME); //should consider a way of merging the log with the rest of the game log
	Ogre::String lResFileName = CONFIGURATION(DS_OGRE_RESOURCES_FILE_NAME);

	LOG(INFO) << "Ogre files: plugins " << lPluginsFileName << "; config " << lConfigFileName << "; log " << lLogFileName;
	

	LOG(INFO) << "Creating Ogre root";
	this->ogreRoot = new Root(lPluginsFileName, lConfigFileName, lLogFileName);

	//load default config
	if( !this->ogreRoot->restoreConfig() ) {
		LOG(INFO) << "Could not open config file - fallback on dialog";
		//fallback on the dialog
		ConfigurationStore::getInstance()->overrideSetting(DS_OGRE_SHOW_CONFIG_WINDOW, YES);
	}

	if(CONFIGURATION(DS_OGRE_SHOW_CONFIG_WINDOW) == YES){
		bool ret=this->ogreRoot->showConfigDialog();
		LOG(INFO) << "Configuration from dialog";
		if(ret==false){
			//user wants to termintate
			LOG(INFO) << "User cancelled";
			this->ready=false;
			return;
		}
	}else{
		LOG(INFO) << "Configuration from file";
		//no action required: ogre can do it alone, default config already loaded
	}


	//loading resource file. This code comes from the tutotial basic application
	//it seems that the file must be loaded and "parsed" separately
	ConfigFile cf;
    cf.load(lResFileName);
	String secName, typeName, archName;
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()){
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	LOG(INFO) << "Loaded resource file "+lResFileName;

	//creating scene manager
	//Ogre::ST_EXTERIOR_REAL_FAR may give better performance
	this->ogreSceneMgr=this->ogreRoot->createSceneManager(Ogre::ST_GENERIC, SCENE_MANAGER_NAME);
	LOG(INFO) << "Scene manager created";

	//it is mandatory to open the window BEFORE creating cameras... otherwise there will be exceptions
	this->ogreWindow = this->ogreRoot->initialise(true, DS_APPLICATION_NAME);
	LOG(INFO) << "Ogre window initialized";

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	this->TEMPORARYSceneSetUp();
	LOG(INFO) << "Scene initialized";

	//set up input listening
	this->input->registerToGetInput(this->ogreWindow, this);
	this->ogreRoot->addFrameListener(this->input);
	LOG(INFO) << "Input system ready";

	//things that I do not know why I need...
	this->ogreWindow->setActive(true); 

	//Prepare the timing facilities.
	this->ogreRoot->getTimer()->reset();
	this->lastTime = this->ogreRoot->getTimer()->getMicroseconds();

	//Prepare the helpers.
	this->effects=new EffectsLibrary(this->ogreSceneMgr);

	this->halt=false;
	//inizialization complete
	this->ready=true;
	alreadyExecuted=true;


	return;
}


/**The user may terminate the construction process by canceling at the configuration screen.
Call this function to know if the root instance can be used.*/
bool OgreFacade::isReady(){
	return this->ready;
}


/**Call this to halt the game loop*/
void OgreFacade::endAll(){
	this->halt=true;
	//no other "de-inizialization" required by now.
	LOG(INFO) << "Halting ogre facade.";
}

/**To be called just before starting the main loop.
It ensures that the facade is ready, initializes the scene and cleans the input queue.
@throws runtime_error if not ready to start because initialization not complete */
void OgreFacade::fireUp(){

	//test: init complete?
	if(!ready){
		//DSLOG("Could not start ogre, as init not complete");
		LOG(ERROR) << "Could not start ogre, as init not complete";
		throw runtime_error("Could not start because init not complete");
	}

	//skybox: should have a function on its own!!! <<<<<<<<<<<<<<<<<<<<<<<<
	//to speed up testing, it was disabled in debugging
	#ifndef _DEBUG
		this->ogreSceneMgr->setSkyBox(true, "Skybox", (1 + 10000)/2); //size is in the middle of the HARDCODED clipping distances, that are now managed by the cameraman...
	#endif

	//clear the rubbish input someone may have sent to the "black" window
	this->ogreRoot->clearEventTimes(); 

	return;
}

/**Draws a frame. To be called when rendering is needed (at eache game loop).
Also checks if the window is still open (in case it is not, it SHOULD CALL for a "general halt"
to quit the game). Finally it has the input system move on.
*/
unsigned long OgreFacade::stepFrame(){
	if(!this->ogreWindow->isClosed()){
		//double buffering
		this->ogreWindow->swapBuffers(true);
		//take the input.
		Ogre::WindowEventUtilities::messagePump();

 		this->ogreRoot->renderOneFrame();

		//Timing
		unsigned long timeNow = this->ogreRoot->getTimer()->getMicroseconds();
		unsigned long elapsedTime = timeNow - this->lastTime;
		this->lastTime = timeNow;
		return elapsedTime;

	} else {
		//Close everything.
		//TODO: HOW??
		LOG(WARNING) << "CLOSE EVERITHYING HAS TO BE IMPLEMENTED";
		return 0;
	}
}

/**Creates an entity and assign its nodes and entity object in the graphic component.
Objects are "flat", all nodes are stored just under the root node.
May cosider hierachies for the future.
*/
void OgreFacade::newEntity(gocVisualModel* graphicInfo){
	//create the entity
	string name=((gocLogicBase*)(graphicInfo->getOwner()->getComponent(GameObjectComponent::GOC_TYPE_LOGIC)))->getObjectName();
	Entity* newEntity = this->ogreSceneMgr->createEntity( name, graphicInfo->getMeshFileName() );
	graphicInfo->setEntity(newEntity);

	//create the node
	SceneNode* newNode = this->ogreSceneMgr->getRootSceneNode()->createChildSceneNode(name+NODE_SUFFIX);
	newNode->attachObject(newEntity);
	graphicInfo->setNode(newNode);
	
	//put the object in a node in the origin, in a default position
	graphicInfo->setPosition(Vector3d(0,0,0));
	graphicInfo->setDirection(dsQuaternion::IDENTITY);
}

void OgreFacade::deleteEntity(gocVisualModel* deleteMe){
	this->ogreSceneMgr->getRootSceneNode()->removeAndDestroyChild(deleteMe->getNode()->getName());
	this->ogreSceneMgr->destroyEntity(deleteMe->getEntity());
}

void OgreFacade::newEntity(gocVisualCamera* graphicInfo){
	//create the node and the camera.
	const std::vector<std::string>& names = graphicInfo->getNames();
	
	Camera* lastCamera;
	std::vector<std::string>::const_iterator it;
	for(it=names.begin(); it!=names.end(); ++it){
		SceneNode* newNode = this->ogreSceneMgr->getRootSceneNode()->createChildSceneNode(*it+NODE_SUFFIX);
		Camera* newCamera = this->ogreSceneMgr->createCamera(*it);
		newNode->attachObject(newCamera);
		graphicInfo->addCamera(newCamera);
		lastCamera=newCamera;
	}
	
	//Create a viewport
	Viewport* newViewport = this->ogreWindow->addViewport(lastCamera); //Problem! We need a default cam here!!!
	graphicInfo->assignViewport(newViewport);
}


/**Converts data between the two formats for vectors.*/
Vector3 vector3d2ogre(Vector3d v){
	Vector3 ret(v.x, v.y, v.z);
	return ret;
}

/**Since this is temporary, code quality is VERY low!*/
void OgreFacade::TEMPORARYSceneSetUp(){
	
	

	//real minimum for lighting
	Light* pointLight = this->ogreSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(100,100,0));

	

	//Ogre::Plane plane(Vector3::UNIT_Y, -10);
	//Ogre::MeshManager::getSingleton().createPlane("plane",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,plane,
	//		1500,1500,200,200,true,1,5,5,Vector3::UNIT_Z);
	//this->ogreSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject
	//Ogre::Entity* ent = this->ogreSceneMgr->createEntity("LightPlaneEntity", "plane");
	//this->ogreSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	//	ent->setMaterialName("Ground"); */

	
	      
}

void OgreFacade::activateEffect(const std::string& name, const Vector3d& position){
	this->effects->startEffect(name, position);
}
 

