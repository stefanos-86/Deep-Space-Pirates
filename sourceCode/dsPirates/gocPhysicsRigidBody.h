#pragma once
#include "gocphysics.h"
#include "dsMath.h"

#include "TimedElement.h"

class ForceGenerator;
class TorqueGenerator;

/*This class helps us move between the "havok world" and the "dspirates world". It is our "easy interface"
to the Havok world. Adapter pattern.
It represent a rigid body with its physics parametrs (mass, inertia...) that is subject to forces...
*/

class hkpRigidBody;
class hkpRigidBodyCinfo;
class ICollisionGeometry;

class ForceGenerator;
class TorqueGenerator;

class gocCollidableLogic;

class gocPhysicsRigidBody :
	public gocPhysics, public TimedElement
{
public:
	/**This constructor sets all the initial parameters and link the object to the arbitraty generators of forces and torques
	(that is, to anything that "pushes" the object in a way not computed by havok. Example: engines are not in havok, collision
	forces are - engines will be forece generators.

	Set the force generator to null to say that the object is not subject to any other dynamic other than
	the one caused by havok (e.g. it has no engine, but reacts to collisions).
	
	Notice that the initial velocity and angular velocity are assumed to be zero.
	This object will NOT deallocate any pointer - it uses, but does not hold, them.

	@param force helper object to generate the forces (local coordinates) Set to null to have a "engineless" object (e.g. a piece of space junk).
	@param torque helper object to generate the forces (local coordinates)
	@param mass mass of the object
	@param initialPosition where the object starts at the beginning of the simulation (default in the origin)
	@param initialOrientation how the object is turned at the beginning of the simulation (default to pointing in the positive X)
	@param initialVelocity initial velocity of the object
	@param collisionGeometryHolder something that manages and provide a shape for collision detection. This object TAKES OWNERSHIP of the geometry!
	@param listener the object that knows how to react to collisions. Set to null if this object does not have to report collisions.
	
	*/
	gocPhysicsRigidBody(ForceGenerator* force, TorqueGenerator* torque, float mass,
						Vector3d initialPosition, dsQuaternion initialOrientation, Vector3d initialVelocity, ICollisionGeometry* collisionGeometryHolder,
						gocCollidableLogic* listener);

	/*Deletes owned objects.*/
	~gocPhysicsRigidBody();

	/**Returns an Havok rigid body. Called at registration time.*/
	hkpRigidBody* getRigidBody() const;

	/**Returns the position of the objects. The vector contains the X, Y and Z coordinates of the object at the current simulation step.
	Converts from havok to dspirates types.
	@return a vector with the position of the object*/
	Vector3d getPosition();

	/**Return the orientation as a quaternion, [relative to the initial orientation - maybe, still studying this]
	Converts from havok to dspirates types.*/
	dsQuaternion getOrientation();

	/**Returns the velocity in WC and dsPirates types.*/
	Vector3d getVelocity();

	/**Returns the velocity in local coordinates and dsPirates types.*/
	Vector3d getLocalVelocity();

	/**Update the forces, torques...*/
	void timeStep(float duration);

	/**Writes into the parameters the angular velocvities (in whatever stuff per second havok coputes them) relative to the local 
	spaceship axes.	Roll, pitch, yaw rates as they are the moment the function is called.*/
	void getAngularVelocities(double& pitchRate, double& rollRate, double& yawRate);

private:
	/**Here we hold the pointer to the havok object that is the underlying implementation
	of the rigid object. We HOLD it (that is, we allocate and free it).*/
	hkpRigidBody* hkRigidbody;

	/**Helper object that can provide the calculation of the total force acting on the object. (local coordinates)*/
	ForceGenerator* forceGenerator;
	/**Helper object that can provide the calculation of the total torque acting on the object. (local coordinates)*/
	TorqueGenerator* torqueGenerator;

	/**Keep a reference to the collision geometry provider to delete it.*/
	ICollisionGeometry* collisionGeometryProvider;
};

