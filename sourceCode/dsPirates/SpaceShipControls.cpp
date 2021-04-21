#include "StdAfx.h"
#include "SpaceShipControls.h"

#include "SpaceShipThrusters.h"
#include "Cockpit.h"
#include "WeaponSystem.h"
#include "FlightControlSystem.h"

//allowed actions
//the action names actual strings MUST be the same used in the key mapping file.
#define MOVE_UP "move up"
#define MOVE_DOWN "move down"
#define MOVE_LEFT "move left"
#define MOVE_RIGHT "move right"
#define MOVE_FORWARD "move forward"
#define MOVE_BACKWARD "move backward"
#define ROLL_LEFT "roll left"
#define ROLL_RIGHT "roll right"
#define PITCH_UP "pitch up"
#define PITCH_DOWN "pitch down"
#define YAW_LEFT "yaw left"
#define YAW_RIGHT "yaw right"

#define CYCLE_MFD "cycle mfd"

#define FIRE "fire"

#define TEMP_CREDITS "temporaryCredits" //Temporary code to meet legal requirements.

SpaceShipControls::SpaceShipControls(SpaceShipThrusters* engine,
									 Cockpit* instrumentPanel,
									 WeaponSystem* guns,
									 FlightControlSystem* fcs) :
	engine(engine),
	instrumentPanel(instrumentPanel),
	guns(guns),
	fcs(fcs)
{ 
	/*Programming errors! This class can not work with null pointers!*/
	assert(engine != 0);
	assert(instrumentPanel != 0);
	assert(guns != 0);
	assert(fcs != 0);
}

	//TODO BREAK IN SUB-FUNCTIONS, SIMPLIFY THE IF-ELSE-RETURNS!!!
void SpaceShipControls::doAction(ActionName const& action, bool stop){
	if(action==CYCLE_MFD && stop==false){
		this->instrumentPanel->cycleInfo();
		return;
	}
	
	if(action==FIRE){

		if(stop==true){
			this->guns->ceaseFire();
		}
		else{
			this->guns->openFire();
		}
		return;
	}

	//Temporary code to meet legal requirements.
	if (action == TEMP_CREDITS){
		this->instrumentPanel->toggleCredits();
		return;
	}

	//select the nozzle according to the action
	//TODO: this should be moved in the flight control system to ease the selection of the control mode (manual/auto).
	SpaceShipThrusters::nozzles toBeActuated;

	//unavoidable if-chain... (much, much less cumbersome that polimorphysm: how many action objects should I need?)
	if(action == MOVE_UP){
		toBeActuated=SpaceShipThrusters::UP;
	}else if(action == MOVE_DOWN){
		toBeActuated=SpaceShipThrusters::DOWN;
	} else if(action == MOVE_LEFT ){
		toBeActuated=SpaceShipThrusters::LEFT;
	} else if(action == MOVE_RIGHT){
		toBeActuated=SpaceShipThrusters::RIGHT;
	} else if(action == MOVE_FORWARD ){
		toBeActuated=SpaceShipThrusters::FORWARD;
	} else if(action == MOVE_BACKWARD){
		toBeActuated=SpaceShipThrusters::BACWARD;
	} else if(action == ROLL_LEFT ){
		toBeActuated=SpaceShipThrusters::ROLL_L;
	} else if(action == ROLL_RIGHT){
		toBeActuated=SpaceShipThrusters::ROLL_R;
	} else if(action == PITCH_DOWN){
		toBeActuated=SpaceShipThrusters::PITCH_D;
	} else if(action ==PITCH_UP ){
		toBeActuated=SpaceShipThrusters::PITCH_U;
	} else if(action == YAW_LEFT ){
		toBeActuated=SpaceShipThrusters::YAW_L;
	} else if(action == YAW_RIGHT){
		toBeActuated=SpaceShipThrusters::YAW_R;
	} else
		return; //can't do this action



	//start or stop the related nozzle
	if(stop==false)
		this->engine->on(toBeActuated);
	else
		this->engine->off(toBeActuated);

}

std::vector<ActionName> SpaceShipControls::declareActions(){
	std::vector<ActionName> ret;

	//Engines
	ret.push_back(MOVE_UP);ret.push_back(MOVE_DOWN);
	ret.push_back(MOVE_LEFT);ret.push_back(MOVE_RIGHT);
	ret.push_back(MOVE_FORWARD);ret.push_back(MOVE_BACKWARD);

	ret.push_back(PITCH_UP);ret.push_back(PITCH_DOWN);
	ret.push_back(ROLL_LEFT);ret.push_back(ROLL_RIGHT);
	ret.push_back(YAW_LEFT);ret.push_back(YAW_RIGHT);

	//instrument panel
	ret.push_back(CYCLE_MFD);

	//Weapons
	ret.push_back(FIRE);

	//Temporary code to meet legal requirements.
	ret.push_back(TEMP_CREDITS);

	return ret;
}

bool SpaceShipControls::isMouseListener() const {
	return true;
}
void SpaceShipControls::mouseMove(const MousePosition& currentMousePosition){
	//Call the cockpit to reposition the cursor and give feedback to the pilot.
	this->instrumentPanel->moveDirectionCursor(currentMousePosition.xPixels, currentMousePosition.yPixels); //Bad! Should not do the unboxing, but rather pass the object!
	//Call the engines/autopilot to tell them where the user wants to go.
	this->fcs->setDesiredRotationVelocity(- currentMousePosition.xNorm, - currentMousePosition.yNorm); //-y! Pitch up is a positive rotation, mouse up is a negative coordinate. Similarly for yaw.

	//Compute the position of the throttle and tell the autopilot to set the speed.
	static double linearThrottle = 0;
	static double linearThrottleStep = 0.1;
	linearThrottle += linearThrottleStep * currentMousePosition.wheelSteps;
	if (linearThrottle > 1)
		linearThrottle = 1;
	if (linearThrottle <- 1)
		linearThrottle = -1;
	this->fcs->setDesiredTransaltionVelocity(linearThrottle);
}