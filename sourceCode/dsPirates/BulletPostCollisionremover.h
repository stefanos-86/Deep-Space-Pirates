#pragma once
#include "GameItemRemover.h" //Colliding bullets are removed at post collision time.

#include <string>

/**Provides a way to delete bullets after collision time.*/
class BulletPostCollisionremover : public GameItemRemover
{
public:
	BulletPostCollisionremover(const std::string& bulletName);
	~BulletPostCollisionremover(void);

	void postCollisionRemove();

private:
	std::string bulletName;
};

