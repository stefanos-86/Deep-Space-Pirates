#include "stdafx.h"
#include "gocVisualModel.h"

gocVisualModel::gocVisualModel(const std::string& file_name) : 
														 meshFileName(file_name),
														 ogreNode(NULL),
														 ogreEntity(NULL) 
{ }


gocVisualModel::~gocVisualModel() { }


const gocVisualModel::goc_id_type gocVisualModel::componentID() const {
	return goc_id_type(GameObjectComponent::GOC_TYPE_VISUAL); 
}

Vector3d gocVisualModel::getPosition() const { 
	return ogreNode->getPosition();
}

void gocVisualModel::setPosition(const Vector3d& new_position) {
	ogreNode->setPosition(new_position);
}

Vector3d gocVisualModel::getDirection() const {
	return ogreNode->getOrientation() *  Vector3d(1,0,0); //Changed to the X axis, but should be * Ogre::Vector3::NEGATIVE_UNIT_Z; (except that the front camera goes astray, on the left, then...)
}

void gocVisualModel::setDirection(const dsQuaternion& new_direction) {
	ogreNode->setOrientation(new_direction);
}

Ogre::SceneNode* gocVisualModel::getNode() const {
	return ogreNode;
}

void gocVisualModel::setNode(Ogre::SceneNode* new_node) { 
	ogreNode = new_node;
}

Ogre::Entity* gocVisualModel::getEntity() const {
	return ogreEntity;
}

void gocVisualModel::setEntity(Ogre::Entity* new_entity) {
	ogreEntity = new_entity;
}

std::string gocVisualModel::getMeshFileName() const {
	return meshFileName;
}

Vector3d gocVisualModel::convertInMyLocal(const Vector3d& otherPosition){
	return this->ogreNode->convertWorldToLocalPosition(otherPosition);
}