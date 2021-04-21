#include "StdAfx.h"
#include "gocPhysicsRigidBody.h"
#include "ForceGenerator.h"
#include "TorqueGenerator.h"
#include "Physics/Dynamics/Entity/hkpRigidBody.h"
#include "ICollisionGeometry.h"
#include "Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h"
#include "Physics/Dynamics/Collide/ContactListener/hkpContactListener.h"
#include "gocCollidableLogic.h"

gocPhysicsRigidBody::gocPhysicsRigidBody(ForceGenerator* force, TorqueGenerator* torque, float mass,
						Vector3d initialPosition, dsQuaternion initialOrientation, Vector3d initialVelocity, ICollisionGeometry* collisionGeometryHolder,
						gocCollidableLogic* listener):
	//set helper object
		forceGenerator(force),
		torqueGenerator(torque),
		collisionGeometryProvider(collisionGeometryHolder)
{
	//1st thing: create the object.
	hkpRigidBodyCinfo hkBodyInfo;

	hkBodyInfo.m_shape = collisionGeometryHolder->getCollisionGeometry();

	//Initial position.
	hkBodyInfo.m_position = hkVector4(initialPosition.x, initialPosition.y, initialPosition.z);
	hkBodyInfo.m_rotation = hkQuaternion(initialOrientation.x, initialOrientation.y, initialOrientation.z, initialOrientation.w);
	hkBodyInfo.m_linearVelocity = hkVector4(initialVelocity.x, initialVelocity.y, initialVelocity.z);

	hkBodyInfo.m_motionType =  hkpMotion::MOTION_SPHERE_INERTIA;
	hkBodyInfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;

	//Mass properties.
	hkBodyInfo.m_mass = mass;	 
	hkpMassProperties massProperties; // output variable
	//Use the collision geometry for inertia too.
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(collisionGeometryHolder->getCollisionGeometry(), mass, massProperties);
	hkBodyInfo.setMassProperties(massProperties);
	
	// Elasticity.
	hkBodyInfo.m_restitution = (hkReal) 1;  //Sticky collisions (spaceships blow up, don't bounce...).
	
	this->hkRigidbody = new hkpRigidBody(hkBodyInfo);
	this->hkRigidbody->setAngularDamping(0);

	//Collsion signalling.
	if(listener!=0)
		this->hkRigidbody->addContactListener(listener);

	//Back-pointer to recover "this" game object from the Havok data.
	//The Havok data is all we have during havok callback, we will attempt to dynamic_cast it to 
	//allow colliding objects to "cooperate".
	//I am not fully sure that the cast is portable. Reduce risk with the assertion.
	assert(sizeof(hkLong) == sizeof(listener));
	this->hkRigidbody->setUserData((hkLong) listener);
	
}

gocPhysicsRigidBody::~gocPhysicsRigidBody(){
	delete this->collisionGeometryProvider;
}

hkpRigidBody* gocPhysicsRigidBody::getRigidBody() const{
	return this->hkRigidbody;
}
	
Vector3d gocPhysicsRigidBody::getPosition(){
	hkVector4 pos = this->hkRigidbody->getPosition();
	Vector3d ret(pos(0), pos(1), pos(2));
	return ret;
}

Vector3d gocPhysicsRigidBody::getVelocity(){
	hkVector4 vel = this->hkRigidbody->getLinearVelocity();
	Vector3d ret(vel(0), vel(1), vel(2));
	return ret;
}

Vector3d gocPhysicsRigidBody::getLocalVelocity(){
	hkMatrix3 rotateToLocal = this->hkRigidbody->getTransform().getRotation();
	if (rotateToLocal.invert(0.001) == HK_FAILURE)
		throw std::runtime_error("Could not invert the matrix well enough. Bad epsilon?");

	hkVector4 wcVelocity = this->hkRigidbody->getLinearVelocity();
	hkVector4 localVelocity;
	rotateToLocal.multiplyVector(wcVelocity, localVelocity);

	Vector3d ret(localVelocity(0), localVelocity(1), localVelocity(2));
	return ret;
}

dsQuaternion gocPhysicsRigidBody::getOrientation(){
	hkRotation toWC=this->hkRigidbody->getTransform().getRotation();
	hkQuaternion orientation(toWC);
	dsQuaternion ret(orientation(3),orientation(0),orientation(1),orientation(2));
	return ret;
}

void gocPhysicsRigidBody::timeStep(float duration){
	//Special stop for objects not subject to forces.
	if(this->forceGenerator == 0){
		return;
	}
		

	//Get data from helpers.
	Vector3d totalForce = this->forceGenerator->getForce(); //the generator works in local coordinates
	Vector3d totalTorque = this->torqueGenerator->getTorque();

	//compute the force and torque in WC - the engines are local to the objects.
	 hkRotation toWC=this->hkRigidbody->getTransform().getRotation();

	 hkVector4 localTorque(totalTorque.x,totalTorque.y,totalTorque.z);
	 hkVector4 localForce(totalForce.x,totalForce.y,totalForce.z);

	 hkVector4 globalForce, globalTorque;
	 toWC.multiplyVector(localForce, globalForce);
	 toWC.multiplyVector(localTorque, globalTorque);
 
	//Pass the data to havok.
	this->hkRigidbody->applyForce((hkReal) duration, globalForce);
	this->hkRigidbody->applyTorque((hkReal) duration, globalTorque);

	
}

void gocPhysicsRigidBody::getAngularVelocities(double& pitchRate, double& rollRate, double& yawRate)
{
	//Get the havok world to local transformation for rotations.
	hkRotation toWC=this->hkRigidbody->getTransform().getRotation();
	toWC.transpose();

	//Get the angular velocity from havok, transform in local reference and return.
	hkVector4 loacalAngVel;
	toWC.multiplyVector(this->hkRigidbody->getAngularVelocity(), loacalAngVel);
	rollRate=loacalAngVel(0);
	yawRate=loacalAngVel(1);
	pitchRate=loacalAngVel(2);
}