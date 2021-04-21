#pragma once
#include "ICollisionGeometry.h"

class hkpShape; 

/**This class returns a very simple collision shape (spherical).
Useful for test dummies or very simple objects that do not require anything more complex.*/
class BasicCollisionGeometry :	public ICollisionGeometry
{
public:
	BasicCollisionGeometry(double sphereRadius);  //Could add a "get by name" function: BasicCollisionGeometry(bullet); Useful?
	~BasicCollisionGeometry(void);

	virtual hkpShape* getCollisionGeometry();

private:
	///This objects OWNS the shape.
	hkpShape* shape;
};

