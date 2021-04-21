#pragma once

#include <map>

#include "gocVisualCamera.h"
#include "ActionProviderPlugin.h"

//This component manages the camera.
namespace Ogre { 
	class Camera;
	class SceneNode;
	class Viewport;
}

/**This class contains the logic to manage the camera. It is in any case a visual component, as it encapsulates
OGRE classes.

This class also provides the commands to move the camera. TODO: may want to factor out this function in the future.
*/
class Cameraman : public gocVisualCamera, public ActionProviderPlugin
{
public:
	/**This constructor will automatically set up the camera.
	It must be called only when the ogre facade is ready (after the init method in the facade).*/
	Cameraman();

	//Implementation of the gocVisualCamera interface.
	virtual void addCamera(Ogre::Camera* camera);
	virtual void assignViewport(Ogre::Viewport* viewport);
	virtual const std::vector<std::string>& getNames();

	//Implementation of the action provider interface.
	virtual void doAction(ActionName const& action, bool stop);
	virtual std::vector<ActionName> declareActions();

private:
	/**Pointers to the available ogre cameras, stored by name. This object is not the owner of any camera!*/
	std::map<std::string,Ogre::Camera*> cameras;
	/**Current viewport for rendering. This object is not the owner!*/
	Ogre::Viewport* viewport;

};

