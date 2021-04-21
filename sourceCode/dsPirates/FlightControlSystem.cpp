#include "StdAfx.h"
#include "FlightControlSystem.h"


#include "SpaceShipStatus.h"

static const double kDesiredVelocitySteps = 10; //There will be this many steps forward and this many steps in reverse.

FlightControlSystem::FlightControlSystem(SpaceShipThrusters& engines, double maxTranslationVelocity, double maxRotationalVelocity) :
	engines(engines),
	kMaxRotationVelocity(maxRotationalVelocity),
	kMaxTraslationVelocity(maxTranslationVelocity),
	//Initially the ship is standing still.
	desiredYawRate(0),		
	desiredPitchRate(0),
	desiredVelocity(0)
{
}


FlightControlSystem::~FlightControlSystem(void)
{
}

///Maps normalizedValue from [-1, +1] to [-maxValue, +maxValue] (converts from "throttle settings" to rad/s).
static double throttleSettingToRotationRate(double normalizedValue, double maxValue){
	return normalizedValue * maxValue;
}

///Maps the vector with the normalized components to a vector in m/s.
static double throttleSettingsToTranslationVelocity(double throttleSpeed, double maxSpeed){
	return throttleSpeed * maxSpeed;
}

void FlightControlSystem::setDesiredRotationVelocity(double desiredYawRate, double desiredPitchRate){
	//Check expected input range.
	assert(-1 <= desiredYawRate && desiredYawRate <= +1);
	assert(-1 <= desiredPitchRate && desiredPitchRate <= +1);

	this->desiredYawRate = throttleSettingToRotationRate(desiredYawRate, kMaxRotationVelocity);
	this->desiredPitchRate = throttleSettingToRotationRate(desiredPitchRate, kMaxRotationVelocity);

	//Check that the values don't go over the maximum (now is a trivial computation, but in the future...).
	assert(-kMaxRotationVelocity <= this->desiredYawRate && this->desiredYawRate <= +kMaxRotationVelocity);
	assert(-kMaxRotationVelocity <= this->desiredPitchRate && this->desiredPitchRate <= +kMaxRotationVelocity);
}

void FlightControlSystem::setDesiredTransaltionVelocity(double desiredSpeed){
	//Check expected input range.
	assert(-1 <= desiredSpeed && desiredSpeed <= +1);
	this->desiredVelocity = throttleSettingsToTranslationVelocity(desiredSpeed, kMaxTraslationVelocity);

	//Check that it did not go over the maximum (now is a trivial computation, but in the future...).
	assert(-kMaxTraslationVelocity <= desiredVelocity && desiredVelocity <= +kMaxTraslationVelocity);
}


//Avoid micro-corrections and deals with approximation.
static bool areVeryClose(double a, double b){
	static double kTolerance = 0.001;   //Set this to a big value (0.1) to cause oscillations (could be useful in the "broken" mode, if ever implemented);
	double difference = a - b;
	return (-kTolerance < difference) && (difference < kTolerance);
}

void FlightControlSystem::actuate(const SpaceShipStatus& status){
	//Rotations.
	this->actuateSingleAxis(status.rollRate, 0, SpaceShipThrusters::ROLL_L, SpaceShipThrusters::ROLL_R);
	this->actuateSingleAxis(status.pitchRate, desiredPitchRate, SpaceShipThrusters::PITCH_U, SpaceShipThrusters::PITCH_D);
	this->actuateSingleAxis(status.yawRate, desiredYawRate, SpaceShipThrusters::YAW_L, SpaceShipThrusters::YAW_R);
	
	//Linear movement.
	this->actuateSingleAxis(status.linearLocalVelocity.y, 0, SpaceShipThrusters::UP, SpaceShipThrusters::DOWN); 
	this->actuateSingleAxis(status.linearLocalVelocity.z, 0, SpaceShipThrusters::RIGHT, SpaceShipThrusters::LEFT); 
	this->actuateSingleAxis(status.linearLocalVelocity.x, this->desiredVelocity, SpaceShipThrusters::FORWARD, SpaceShipThrusters::BACWARD); 
}

void FlightControlSystem::actuateSingleAxis(double actual, double desired, SpaceShipThrusters::nozzles ifNegative, SpaceShipThrusters::nozzles ifPositive){
	if (! areVeryClose(actual, desired)) //need correction?
	{
		if (actual > desired){ //actual - desired > 0
			this->engines.off(ifNegative);
			this->engines.on(ifPositive);
		} else {
			this->engines.off(ifPositive);
			this->engines.on(ifNegative);
		}
	}	
}