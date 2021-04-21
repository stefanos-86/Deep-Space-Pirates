#pragma once

#include <string>
#include "gocCollidableLogic.h"
#include "TimedElement.h"

/**Logic class for machine gun bullets. It handles their collisions and updates the graphic position, gives the physics time steps...*/
class Bullet : public TimedElement, public gocCollidableLogic
{
public:
	Bullet(const std::string& name, int power);
	~Bullet(void);

	///Keep graphic and physics aligned.
	virtual void timeStep(float stepDuration);
	
	///Self-destroy after a certain time.
	virtual void timedCallback(numeric_id_t id, float actualDuration);

	/**Implements havok collisions. On collision the bullet is removed from the game.
	Remove this if you want bullet that bounce.*/
	virtual void contactPointCallback( const hkpContactPointEvent& event );

	/**Allows the target to know how "powerful" the bomb is.
	Provides a extra damage value to be removed from the target healt points.
	TODO: this assumes that HP values are a "currency" - that all the values are balanced etc etc.
	But is simple!*/
	int getWeaponDamage();

private:
	///How powerful the round is (fixed extra damage added to the collision effects);
	int power;
};

