#include "stdafx.h"

#pragma once
#include "gocvisual.h"
#include "dsMath.h"

class gocVisualModel :
	public gocVisual
{

public:

	gocVisualModel(const std::string& mesh_file_name);
	
	virtual ~gocVisualModel();

	/**Returns the type of the component.
	@returns the object type*/
	virtual const goc_id_type componentID() const;

	Vector3d getPosition() const;

	void setPosition(const Vector3d& new_position);

	Vector3d getDirection() const;

	void setDirection(const dsQuaternion& new_direction);

	Ogre::SceneNode* getNode() const;

	void setNode(Ogre::SceneNode* new_node);

	Ogre::Entity* getEntity() const;

	void setEntity(Ogre::Entity* new_entity);

	std::string getMeshFileName() const;

	//This helper is intended to support the radar.
	//Assuming that otherPosition is a vector that gives the position of
	//some object in WC, it returns the position of the objects in the local components of this visual object.
	//It stays there because it is convenient to use convertWorldToLocalPosition - in the Ogre library.
	//(Maybe it would be better to use the havok orientation? Or another method?)
	Vector3d convertInMyLocal(const Vector3d& otherPosition);
	
private:

	std::string meshFileName;
	
	Ogre::SceneNode* ogreNode;
	
	Ogre::Entity* ogreEntity;
};

