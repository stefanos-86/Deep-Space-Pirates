#include "StdAfx.h"
#include "gocLogicBase.h"

gocLogicBase::gocLogicBase(const std::string& name) : objectName(name)
{}

const std::string gocLogicBase::getObjectName(){
	return this->objectName;
}
