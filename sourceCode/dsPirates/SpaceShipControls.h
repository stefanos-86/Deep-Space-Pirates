#pragma once
#include "ActionProviderPlugin.h"

#include "ActionName.h"

/**The virtual spaceship equivalent of the dashboard. It is the spaceship "user interface": buttons and levers in the
physical thing, a "device" that gets the user input in the virtual thing.

Why not using the spaceship subsystems as input plugins? To do "fancy things" with input (e.g. simulate damage and broken controls,
to mirror more easily the control settings in the dashboard on the screen...).
Ans to reduce duplication of "input handling code" in all the sub-systems.
*/

class SpaceShipThrusters;
class Cockpit;
class WeaponSystem;
class FlightControlSystem;

class SpaceShipControls : public ActionProviderPlugin
{
public:
	SpaceShipControls(SpaceShipThrusters* engine, Cockpit* instrumentPanel, WeaponSystem* guns, FlightControlSystem* fcs);

	//satisfy the input provider interface
	virtual void doAction(ActionName const& action, bool stop);
	virtual std::vector<ActionName> declareActions();
	virtual bool isMouseListener() const;
	virtual void mouseMove(const MousePosition& currentMousePosition);

private:
	/**The controls give the input to the engine.*/
	SpaceShipThrusters* engine;

	/**We also control the info display.*/
	Cockpit* instrumentPanel;

	/**Weaponry*/
	WeaponSystem* guns;

	/**Engine governor/fly-by-wire and other "driving aids".*/
	FlightControlSystem* fcs;

	//later subsistems, later on...
};

