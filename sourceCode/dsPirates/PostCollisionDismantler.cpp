#include "StdAfx.h"
#include "PostCollisionDismantler.h"

#include "GameItemRemover.h"

PostCollisionDismantler::PostCollisionDismantler(void)
{
}


PostCollisionDismantler::~PostCollisionDismantler(void)
{
}

void PostCollisionDismantler::postCollideCallback( hkpWorld* world, const hkStepInfo& stepInfo ){
	for(std::vector<GameItemRemover*>::iterator it = this->objectsPendingDeletion.begin();
		it != this->objectsPendingDeletion.end(); ++it){
			(*it)->postCollisionRemove();
			delete (*it);
	}
	this->objectsPendingDeletion.clear();
}


void PostCollisionDismantler::registerForPostCollisionDeletion(GameItemRemover* remover){
	this->objectsPendingDeletion.push_back(remover);
}