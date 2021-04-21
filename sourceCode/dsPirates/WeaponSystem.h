#pragma once

#include "TimedElement.h"
#include "IDGenerator.h"

class BulletCreator;

/**This class represents the weapon system of the spaceship and implement the 
technical steps to realize it (create bullet objects, set their speed, register them in havok...)"

In the future it may handle munition counts, targeting...
By now it only fires a gun.
*/
class WeaponSystem : public TimedElement
{
public:
	WeaponSystem(void);
	~WeaponSystem(void);

	///Allows the main spaceship logic to hook up this

	///Start firing the gun.
	void openFire(void);
	///Stop firing the gun.
	void ceaseFire(void);

	///Firing "tempo" for the machine gun (fire a round at each call).
	void timedCallback(numeric_id_t id, float actualDuration);

private:
	/**Helper object to create game objects for the bullets, missiles...
	This class OWNS the object.*/
	BulletCreator* bulletCreator;

	/**The player may be able to push the firing button so fast that he can shot faster
	than the full auto mode. This acts as a lock, The gun becames not ready after firing a shot.
	It is restored to ready only when the gun callback is fired*/
	bool gunReady;
	bool gunFiring;

	///Helper to give unique names to the bullets.
	IDGenerator machineGunBulletsIdMaker;
};

