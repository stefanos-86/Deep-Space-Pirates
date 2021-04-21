#pragma once

/**During the collision computation phases some objects may have to be removed from the system.
Typical case: a projectile that hit the target. Since we have no control on the order of collision notification
we have to ensure that if A and B collide then A and B are both available to each other during their call from havok.
If one of them is to be removed, then it will register itself for removal and this class will hanlde it after the 
collision resolution phase.*/

#include <vector>
#include <Physics/Dynamics/World/Listener/hkpWorldPostCollideListener.h>

class GameItemRemover;

class PostCollisionDismantler : public hkpWorldPostCollideListener
{
public:
	///Implements the hkpWorlPostCollideListener interface
	virtual void postCollideCallback( hkpWorld* world, const hkStepInfo& stepInfo );
	
	///Allows to register a object for the deletion phase. TAKES OWNERSHIP of the deletor.
	void registerForPostCollisionDeletion(GameItemRemover* remover);

	PostCollisionDismantler(void);
	~PostCollisionDismantler(void);

private:
	std::vector<GameItemRemover*> objectsPendingDeletion;
};

