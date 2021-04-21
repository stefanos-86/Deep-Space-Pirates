#pragma once

/**This interface is implemented by classes that provide a collision geometry for use by havok rigid bodies.
The gocPhysicsRigidBody expect one such provider as a parameter.
Implementations may compute and return the geometry in any way they want (load from file, use Havok shapes...),
as long as a hkShape is made available.

Implementations are supposed to OWN the data that describe the geometry (hkShape, any data in memory etc. etc.).
*/

class hkpShape;

class ICollisionGeometry
{
public:
	virtual hkpShape* getCollisionGeometry() = 0;
	virtual ~ICollisionGeometry() {};
};

