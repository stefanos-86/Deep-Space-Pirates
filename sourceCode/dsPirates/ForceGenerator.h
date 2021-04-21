#include "stdafx.h"


#pragma once

#include "dsMath.h"

/**This interface abstracts all objects that are capable of computing a force for physics calculations.*/

class ForceGenerator {
	public:
	/** (Pure) Virtual destructor, in order to make this class pure abstract.*/
	virtual ~ForceGenerator() = 0;

	/**Call this method to get the total force out of the force generator.*/
	virtual Vector3d getForce() = 0;
};

inline ForceGenerator::~ForceGenerator(){};