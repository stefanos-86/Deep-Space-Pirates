#pragma once


#include <string>
#include "dsMath.h"

/**This class handle the constructions of weapons projectiles, missiles... and the usual registration in the goc, havok, ogre systems.
It also handle the dismantling, as it has to do the same steps in reverse (plus this may allow to implement pools of objects,
flyweight pattern... should it become necessary).*/
class BulletCreator 
{
public:
	BulletCreator(void);
	~BulletCreator(void);

	void createGunBullet(const Vector3d& initialPosition,
					     const dsQuaternion& initialOrietation,
						 const Vector3d& velocity,
						 const std::string& name,
						 const float selfDestroyTime);

	void dismantleGunBullet(const std::string& name);


};

