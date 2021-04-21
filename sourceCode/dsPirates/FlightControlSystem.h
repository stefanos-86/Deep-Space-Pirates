#pragma once

class SpaceShipThrusters;
struct SpaceShipStatus;
#include "SpaceShipThrusters.h" //Can not forward-declare the nozzle enum.

/**This class is the "driver aid" of the spaceship. It adds some niceties like an auto rotation control,
an auto velocity vector control and so on. It gives input to the spaceship trusters, does not move anything directly.*/
class FlightControlSystem
{


public:
	/**Creates a control system object that controls the given engines and does not allow velocities
	higher than the specified ones on each axis.*/
	FlightControlSystem(SpaceShipThrusters& engines,double maxTranslationVelocity, double maxRotationalVelocity);
	~FlightControlSystem(void);

	/**Set the desired speed for the control system to follow.
	All parameters must be between -1 and 1.
	It directly commands the engines to achieve the desired effect.
	@param desiredYawRate is desired the fraction of the maximum rotation velocity. Negative values are to yaw left.
	@param desiredPitchRate is the desired fraction of the maximum rotation velocity. Negative values are to pitch down.
	*/
	void setDesiredRotationVelocity(double desiredYawRate, double desiredPitchRate);

	/**Set the desired translation speed on the local X axis for the control system to follow.
	The local Y and Z axis velocities are kept to 0. This "pushes the ship forward".
	The parameter represents the velocity in throttle settings: in the [-1;+1] interval.
	A negative value means going in reverse. +1 is the maximum forward speed.
	*/
	void setDesiredTransaltionVelocity(double desiredSpeed);

	/**Increase the desired translation speed by a fixed step.
	@see setDesiredRotationVelocity*/
	void increaseDesiredTransaltionVelocity();

	/**Decrease the desired translation speed by a fixed step.
	@see setDesiredRotationVelocity*/
	void decreaseDesiredTransaltionVelocity();

	/**Computes the engine activation to get closer to the desired rotation and velocity.
	It should be called at every loop, or at least regularly in time.
	The logic is simple: compute the difference between the desired and the actual velocities, activate the engines to 
	reduce it.*/
	void actuate(const SpaceShipStatus& status);


private:
	///The output of the FlightControlSystem are commands for the engines.
	SpaceShipThrusters& engines;
	
	/**Maximum allowed rotational speed around an axis (use manual commands to bypass the governor and... overboost!).
		Is the same on all axis.*/
	double kMaxRotationVelocity;

	/**Maximum allowed traslation speed along an axis (use manual commands to bypass the governor and... overboost!).
	Is the same on all axis.*/
	double kMaxTraslationVelocity;

	///The yaw rate that must be obtained (rad/s).
	double desiredYawRate;
	///The pitch rate that must be obtained (rad/s).
	double desiredPitchRate;

	///The translation velocity along the X axis that must be obtained (m/s).
	double desiredVelocity;

	/**Correct the rotation on an axis (actuates the desired engine).
	If the difference between actual and desired is negative, it switches on ifNegative, otherwise ifPositive.
	*/
	void actuateSingleAxis(double actualn, double desired, SpaceShipThrusters::nozzles ifNegative, SpaceShipThrusters::nozzles ifPositive);

};

