#pragma once
#include "GameItemRemover.h"
#include <string>

/**Provides a way to delete "space garbage" after collision time.
TODO: interface very, very similar to the BulletPostCollisionRemover -> refactor toghether?*/
class DebrisPostCollisionRemover : public GameItemRemover
{
public:
	DebrisPostCollisionRemover(const std::string& objectName);
	~DebrisPostCollisionRemover(void);

	void postCollisionRemove();

private:
	std::string objectName;
};
