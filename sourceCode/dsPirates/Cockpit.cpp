#include "StdAfx.h"
#include "Cockpit.h"

//Manages the Ogre display stuff.
#include "CockpitScreenElements.h"

//Compose the text (stringstream).
#include <string>

//Data to be seen by the user
#include "SpaceShipStatus.h"

using namespace Ogre;

static const size_t lineSize = 15; //How many chars in a line (excluding the final \0 when using c functions);

//TODO: support screen resizing (e.g. delete and rebuild the instruments in different positions).

Cockpit::Cockpit(const OgreFacade& ogreFacade) : //The facade must be a parameter so it is sure that it is up and running before construction.
	currentScreen(infoScreens::rotationRateScreen),
	broken(false),
	masterAlarmActive(false),
	masterAlarmBlink(true),
	dataBuffer(0),
	screenElements(new CockpitScreenElements()),
	radarDotsIDMaker("RadarDot")
{
	//Set initial status of the master alarm.
	this->screenElements->setMasterAlarmOff();
	 
	 //Credits are hidden. TEMPORARY!!!
	this->screenElements->hideCredits();
}

Cockpit::~Cockpit(void){
	delete this->screenElements;
	//Ogre owns the widgets, the spaceship owns the status - nothing else to destroy.
}

void Cockpit::timeStep(float stepDuration){
	//Update the shown information:
	if(this->currentScreen==Cockpit::rotationRateScreen)
		this->rollRates();
	else
		this->stats();

}

void Cockpit::timedCallback(numeric_id_t id, float actualDuration){
	//Blink the master alarm;
	if(this->masterAlarmActive)
	{
		if(this->masterAlarmBlink){
			//Is on, turn off
			this->screenElements->setMasterAlarmOn();
			masterAlarmBlink=false;
		} else {
			//On a random basis we get the "oh shit" message
			if(rand() % 10 == 0){
				this->screenElements->setMasterAlarmAlternative();
			}
			else
			{
				//Is off, turn on
				this->screenElements->setMasterAlarmOff();
			}
			masterAlarmBlink=true; //Whatever it displays, the status changed.
		}
	}
}

void Cockpit::masterAlarmOff(){
	this->masterAlarmActive=false;
}

void Cockpit::masterAlarmOn(){
	this->masterAlarmActive=true;
}
	
///Replaces random chars in the string with blanks.
static void RandomBlanks(std::string& text){
	static const unsigned int kMaximumBrokenChars = 7; //Tweak to decide how many chars to override.
		size_t lastPos=text.size();
		for(int i=0; i<kMaximumBrokenChars; i++){
			int position=rand() % lastPos;
			text.at(position) = ' ';
		}
}

void Cockpit::rollRates(){
	char lineBuffer[lineSize+1];
	char lineFormat[]="X %0+10.5f";

	memset(lineBuffer, 0, lineSize);
	sprintf(lineBuffer, lineFormat, this->dataBuffer->pitchRate);
	lineBuffer[0]='P';
	std::string pitchLine(lineBuffer);

	memset(lineBuffer, 0, lineSize);
	sprintf(lineBuffer, lineFormat, this->dataBuffer->rollRate);
	lineBuffer[0]='R';
	std::string rollLine(lineBuffer);

	memset(lineBuffer, 0, lineSize);
	sprintf(lineBuffer, lineFormat, this->dataBuffer->yawRate);
	lineBuffer[0]='Y';
	std::string yawLine(lineBuffer);

	//Broken stuff override - some digits won't work.
	//Override them with randomly placed blanks.
	if(this->broken){
		RandomBlanks(pitchLine);
		RandomBlanks(rollLine);
		RandomBlanks(yawLine);
	}

	this->screenElements->printOnMFD(pitchLine, rollLine, yawLine);
}

void Cockpit::stats(){
	//Healt points.
	char lineBuffer[lineSize+1];
	memset(lineBuffer, 0, lineSize + 1);

	char lifeLineFormat[]="Life % 3.0f%%";
	float healthPercentage = (float) this->dataBuffer->health / this->dataBuffer->maxHealth * 100;
	//snprintf(lineBuffer, lineSize, lifeLineFormat, healthPercentage); //Not supported! http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
	sprintf(lineBuffer, lifeLineFormat, healthPercentage);
	std::string lifeLine(lineBuffer);

	//Linear speed
	memset(lineBuffer, 0, lineSize);
	char speedLineFormat[] = "Speed %0+7.2f";
	double speed = this->dataBuffer->linearLocalVelocity.x;
	sprintf(lineBuffer, speedLineFormat, speed); 
	std::string speedLine(lineBuffer); 

	
	if(this->broken){
		RandomBlanks(lifeLine);
		RandomBlanks(speedLine);
	}

	this->screenElements->printOnMFD(lifeLine, speedLine, "Score n/a");
}

void Cockpit::setDisplayData(SpaceShipStatus * status){
	this->dataBuffer=status;
}

void Cockpit::cycleInfo(){
	if(this->currentScreen == Cockpit::rotationRateScreen)
		this->currentScreen=Cockpit::statusScreen;
	else
		this->currentScreen=Cockpit::rotationRateScreen;
}

void Cockpit::setBroken(){
	this->broken = true;
}

void Cockpit::addRadarContact(double horizontalAngle, double verticalAngle, bool front){
	this->screenElements->addRadarDot(
		horizontalAngle,
		verticalAngle,
		front,
		this->radarDotsIDMaker.getNewStringId());
}

void Cockpit::cleanRadar(){
	this->screenElements->removeRadarDots();
}

void Cockpit::moveDirectionCursor(int absoluteX, int absoluteY){
	this->screenElements->positionDirectionPointer(absoluteX, absoluteY);
}

//Temporary code to meet legal requirements. TEMPORARY!!!
void Cockpit::toggleCredits(){
	static bool creditsAreVisible = false;
	if (!creditsAreVisible){
		this->screenElements->showCredits();
		creditsAreVisible = true;
	} else {
		this->screenElements->hideCredits();
		creditsAreVisible = false;
	}
}