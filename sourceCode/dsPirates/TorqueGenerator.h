#include "stdafx.h"
#pragma once

/**This interface abstracts all objects that are capable of computing a torque for physics calculations.*/
class Vector3d;

class TorqueGenerator {
	public:
	/** (Pure) Virtual destructor, in order to make this class pure abstract.*/
	virtual ~TorqueGenerator () = 0;

	/**Call this method to get the total torque out of the force generator.*/
	virtual Vector3d getTorque() = 0;
};

inline TorqueGenerator::~TorqueGenerator(){};