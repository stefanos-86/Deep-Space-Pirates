#pragma once
/**This class automates the creation process for "dumb" objects in space (asteroids, debris...).
It offer methods to create varioue (BY NOW ONLY TEST OBJECTS) kinds of objects. It deals with all the goc assembly process
and the registration of facades.
For this reason no object or component is returned: they are available in the gocManager after creation.
*/
class SpaceDebrisCreator
{
public:

	/**Creates a collision crash-test wall like target with the given name at the desired position.
	It can be hit in collisions and floats freely in space.*/
	void createCollisionTarget(std::string name, double x, double y, double z);
	/**Ensure that the collision targets are properly un-registered from the facades etc. etc.
	Erase all of the objects of the given names.*/
	void dismantleCollisionTarget(const std::vector<std::string>& names);
	/**Same as dismantleCollisionTarget(const std::vector<std::string>& names), for a single object.*/
	void dismantleCollisionTarget(const std::string& name);

	/**Creates the reference axes. The three cartesian axes, centered in the origin. Can't be collided with.*/
	void createReferenceAxes();

	/**Creates the reference axes. The three cartesian axes, centered in the origin. Can't be collided with.*/
	void createReferenceCompass();
};

