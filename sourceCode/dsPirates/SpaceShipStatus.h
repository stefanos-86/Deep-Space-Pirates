#pragma once

#include "dsMath.h" //Velocity vector;

/**This simple data object contains the status of the spaceship.
This status is "acted upon" by the logic. The cockpit too can read it.
(this avoid mutual dependencies between logic and cockpit - the final situation is:

SpaceShip --has a-->Cockpit (and other stuff)
    |                    |
	|                    |reads
    +-has a --> Status <-+
).

*/

struct SpaceShipStatus {
	///From a maximum value set at construction to 0
	int health;
	///Initial value of health, to compute the percentage
	int maxHealth;
	///Current rotation rate of the spaceship.
	double pitchRate, rollRate, yawRate;

	///Current linear velocity (local coordinates).
	Vector3d linearLocalVelocity;
};