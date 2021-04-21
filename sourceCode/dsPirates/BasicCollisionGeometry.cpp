#include "StdAfx.h"
#include "BasicCollisionGeometry.h"

#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>

BasicCollisionGeometry::BasicCollisionGeometry(double sphereRadius){
	this->shape = new hkpSphereShape(sphereRadius);
}


BasicCollisionGeometry::~BasicCollisionGeometry(void){
	delete this->shape;
}

hkpShape* BasicCollisionGeometry::getCollisionGeometry(){
	return this->shape;
}