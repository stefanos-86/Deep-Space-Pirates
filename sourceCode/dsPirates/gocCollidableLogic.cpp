#include "StdAfx.h"
#include "gocCollidableLogic.h"


gocCollidableLogic::gocCollidableLogic(const std::string& name) :
	gocLogicBase(name)
{
}


gocCollidableLogic::~gocCollidableLogic(void)
{
}

void gocCollidableLogic::contactPointCallback( const hkpContactPointEvent& theEvent ){
	//Intentionally do nothing
}