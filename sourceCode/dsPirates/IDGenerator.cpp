#include "StdAfx.h"
#include "IDGenerator.h"


void IDGenerator::init(){
	this->counter=0;
}

IDGenerator::IDGenerator(void) :
	idPrefix("") 
	{
		init();
	}
	
IDGenerator::IDGenerator(const std::string& idPrefix) :
	idPrefix(idPrefix)
	{
		init();
	}

numeric_id_t IDGenerator::getNewNumericId(){
	return this->counter++;
}

std::string IDGenerator::getNewStringId(){
	std::stringstream stringId;
	stringId << this->idPrefix << this->counter;
	++counter;
	return stringId.str();
}