#include "StdAfx.h"
#include "ShipSound.h"

#include "dsSound.h"
#include "FMODfacade.h"

ShipSound::ShipSound(void)
{
	//Load the sound clips.
	//BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!
	//Still more hardcoded paths!!!
	this->masterAlarm = FMODfacade::getInstance()->getLoopSound("..\\..\\media\\dsp\\audio\\masterAlarm.ogg");
	this->collision = FMODfacade::getInstance()->getSound("..\\..\\media\\dsp\\audio\\impact.ogg");
	this->gun = FMODfacade::getInstance()->getSound("..\\..\\media\\dsp\\audio\\gunShot.ogg");
}


ShipSound::~ShipSound(void)
{
	delete this->masterAlarm;
	delete this->collision;
	delete this->gun;
}


void ShipSound::startMasterAlarm()
{
	FMODfacade::getInstance()->play(*(this->masterAlarm));
}

void ShipSound::collisionSound()
{
	FMODfacade::getInstance()->play(*(this->collision));
}

void ShipSound::gunShot(){
	FMODfacade::getInstance()->play(*(this->gun));
}
