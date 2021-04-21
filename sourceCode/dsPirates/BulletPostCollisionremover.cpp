#include "StdAfx.h"
#include "BulletPostCollisionremover.h"

#include "BulletCreator.h"

BulletPostCollisionremover::BulletPostCollisionremover(const std::string& bulletName) :
	bulletName(bulletName)
{
}


BulletPostCollisionremover::~BulletPostCollisionremover(void)
{
}

void BulletPostCollisionremover::postCollisionRemove(){
	BulletCreator bc;
	bc.dismantleGunBullet(this->bulletName);
}
