#include "StdAfx.h"
#include "DebrisPostCollisionRemover.h"

#include "SpaceDebrisCreator.h"

//TODO: very similar to the bullet one: how to provide a single "all-purposese" post collision destroyer?
DebrisPostCollisionRemover::DebrisPostCollisionRemover(const std::string& objectName):
	objectName(objectName)
{
}


DebrisPostCollisionRemover::~DebrisPostCollisionRemover(void)
{
}

void DebrisPostCollisionRemover::postCollisionRemove(){
	SpaceDebrisCreator spc;
	spc.dismantleCollisionTarget(this->objectName);
}
