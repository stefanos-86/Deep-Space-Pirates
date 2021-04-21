/**@file */
#pragma once
#include "stdafx.h"


#include "InputMapper.h"
#include "dsMath.h"

#include "EasySingleton.h"

class gocVisualModel;
class gocVisualCamera;
class EffectsLibrary;

/*Some ogre specific constants.
Those should be static class variables, but apparently there is no way of declaring them private. Will keep those defines, then.*/
#define SCENE_MANAGER_NAME "scene_manager"
#define PLAYER_NODE_NAME "player_node"

///prefix to automatically generated identifiers
#define NAME_BASE "Object"
///suffix to add to entity names to identify their nodes
#define NODE_SUFFIX "-node"

/**This class is the entry point in front of Ogre functionalities.
Set up the engine, start it, takes the updated game status, gives back the input to our logic.*/
class OgreFacade 
{
SINGLETON(OgreFacade);
public:
	/**This constructore receives the input object from elsewere.
	This facade can not create the input object, as it does not know the logic parts where the input is directed.
	However, the input object must be destroyed when the window is closed!!! (in the shutdown command??).*/
	OgreFacade();
	~OgreFacade();
	bool isReady();
	void fireUp();
	/**Draws a frame and return the microsecond elapsed since its last call.
	Timing in a portable, thread-safe, high-resolution fashion with CPU affinity is difficult.
	We set the end of a frame rendering as the "timing point".*/
	unsigned long stepFrame();
	void endAll();

	/**Register a mesh in the "ogre system".*/
	void newEntity(gocVisualModel*);
	/**Register a camera in the "ogre system".*/
	void newEntity(gocVisualCamera*);

	/**Unregister a mesh, make it "disappear" from the video.*/
	void deleteEntity(gocVisualModel*);

	void init(InputMapper*);

	/**Activate a visual effect given the name.
	@param name the name of the effect to show
	@param position the position of the effect in WC
	Throws in case the name is unknown.*/
	void activateEffect(const std::string& name, const Vector3d& position);

private:
	//our input system and other helpers.
	InputMapper* input;
	EffectsLibrary* effects;
	//constants for ogre use only
	Ogre::Root *ogreRoot;
	Ogre::RenderWindow* ogreWindow;
	Ogre::SceneManager* ogreSceneMgr; //there can be more than one of those, if you have more that a scene
	
	//Time at last frame;
	unsigned long lastTime;

	///This functions set up the initial scene, to test the engine. MUST be removed as soon as the game state class is ready and we can create the drawing logic
	void TEMPORARYSceneSetUp();
	bool ready;
	bool halt;

	Ogre::Vector3 vector3d2ogre(Vector3d);
};

