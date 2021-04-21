#pragma once
#include "stdafx.h"
#include "gocvisual.h"

namespace Ogre { 
	class Camera;
	class SceneNode;
	class Viewport;
}

/**This interface forces every implementor to provide the methods required by the ogre facade
to delegate the camera at startup.*/
class gocVisualCamera :	public gocVisual
{
public:
    gocVisualCamera();
	virtual ~gocVisualCamera();
	/**Returns the type of the component.
	@returns the object type*/
	virtual const goc_id_type componentID() const;

	/**Allows the ogre facade to create and assign a new camera to the manager.*/
	virtual void addCamera(Ogre::Camera* camera) = 0;
	/**The object must receive a viewport that it can use with its cameras.*/
	virtual void assignViewport(Ogre::Viewport* viewport) = 0;
	/**Return all the names of the available cameras. Allows the ogre facade to create the desired cameras.*/
	virtual const std::vector<std::string>& getNames() = 0;
};

