#include "StdAfx.h"
#include "SpaceShipThrusters.h"

#include "dsMath.h"

SpaceShipThrusters::SpaceShipThrusters(const float force, float torque):
	translationForce(force),
	rotationTorque(torque)
{
	this->switchOff(); //we start with the engines off
}

SpaceShipThrusters::~SpaceShipThrusters() {};

void SpaceShipThrusters::switchOff(){
	this->T=0;
	this->Y=0;
	this->Z=0;
	this->A=0;
	this->E=0;
	this->R=0;
}

void SpaceShipThrusters::forward(){
	this->T=this->translationForce;
}
void SpaceShipThrusters::reverse(){
	this->T= - this-> translationForce;
}
void SpaceShipThrusters::up(){
	this->Y=this->translationForce;
}
void SpaceShipThrusters::down(){
	this->Y= - this->translationForce;
}
void SpaceShipThrusters::left(){
	this->Z=this->translationForce;
}
void SpaceShipThrusters::right(){
	this->Z= - this->translationForce;
}

void SpaceShipThrusters::rollLeft(){
	this->A=this->rotationTorque;
}
void SpaceShipThrusters::rollRight(){
	this->A= - this->rotationTorque;
}
void SpaceShipThrusters::pitchUp(){
	this->E=this->rotationTorque;
}
void SpaceShipThrusters::pitchDown(){
	this->E= - this->rotationTorque;
}
void SpaceShipThrusters::yawLeft(){
	this->R=this->rotationTorque;
}
void SpaceShipThrusters::yawRight(){
	this->R=- this->rotationTorque;
}

Vector3d SpaceShipThrusters::getForce(){
	/*The total force is given by adding up the contributions along the axes*/
	 Vector3d totalForce(this->T, this->Y, this->Z);
	 return totalForce;
}

Vector3d SpaceShipThrusters::getTorque(){
	   Vector3d totalTorque;
	//the orientation of torque vectors is known in local coordinates (right hand rule), the magnitude is in the engine parameters
	Vector3d pitchTorque = this->E* Vector3d::UNIT_Z;
    Vector3d rollTorque = this->A* Vector3d::UNIT_X;
    Vector3d yawTorque = this->R* Vector3d::UNIT_Y;;

	totalTorque = pitchTorque+rollTorque+yawTorque;

	return totalTorque;
}

void SpaceShipThrusters::on(nozzles n){
	//rotate or translate?
	float forceOrTorque=0;
	if(n == ROLL_L || n == ROLL_R || n == PITCH_U || n == PITCH_D || n == YAW_L || n == YAW_R)
		forceOrTorque=this->rotationTorque;
	else
		forceOrTorque=this->translationForce;

	this->nozzleActuation(n, forceOrTorque);
}
void SpaceShipThrusters::off(nozzles n){
	this->nozzleActuation(n, 0);
}


void SpaceShipThrusters::nozzleActuation(nozzles n, float force){
	//pick the right nozzle, record the force/torque
	switch (n){
	case ROLL_L: this->A = force; break;
	case ROLL_R: this->A = -force; break;
	case PITCH_U: this->E = force; break;
	case PITCH_D: this->E = -force; break;
	case YAW_L: this->R = force; break;
	case YAW_R: this->R = -force; break;
	case FORWARD: this->T = force; break;
	case BACWARD: this->T = -force; break;
	case LEFT: this->Z = -force; break;
	case RIGHT: this->Z = force; break;
	case UP: this->Y = force; break;
	case DOWN: this->Y = -force; break;
	}
}