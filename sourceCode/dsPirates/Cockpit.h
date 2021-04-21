#pragma once

/**This class models the spaceship cockpit (monitors, alarm lights etc. etc.).
It encapsulates the Ogre overlays to draw the pilot instruments.
Since some of those blink we need to register to the timing service.*/

#include "gocVisual.h"
#include "TimedElement.h"
#include "IDGenerator.h"
#include <string>

class OgreFacade;
class CockpitScreenElements;
struct SpaceShipStatus;

class Cockpit :  public gocVisual, public TimedElement
{
public:
	/**This constructor builds the cockpit. Even if the ogre facade is not required in the construction
	it is wanted as a parameter to ensure that the facade is up and running before the cockpit is built.*/
	Cockpit(const OgreFacade& ogreFacade);
	~Cockpit(void);

	/**Turn off the master alarm light.*/
	void masterAlarmOff();

	/**Activates the master alarm light.*/
	void masterAlarmOn();

	//In the future: turn on/off other indicators. E. g. low fuel light, handbrake light...

	/**Update the info on the cockpit screen (as fast as possible)*/
	virtual void timeStep(float stepDuration);
	/**1-second timer for blinking lights - set up at sapceship creation.*/
	virtual void timedCallback(numeric_id_t id, float actualDuration);

	/**Set the reference data buffer from which data is read.*/
	void setDisplayData(SpaceShipStatus * status);

	/**Change the displyed information in the instrument panel.*/
	void cycleInfo();

	/**Put the component in "broken mode" - a damaged screen with some chars unreadable.*/
	void setBroken();

	/**Add a radar dot on the screen. Angula coordinates are supposed to be between -1 and 1 (unchecked).
	Set front to true if the contact is in front of the spaceship, rear to paint it on the rear screen.
	This class will convert into the "radar device coordinates".*/
	void addRadarContact(double horizontalAngle, double verticalAngle, bool front);

	/**Remove all the dots from the radar.*/
	void cleanRadar();

	/**Position the mouse cursor at the given screen coordinates.*/
	void moveDirectionCursor(int absoluteX, int absoluteY); //TODO: find a way to pass the mousePosition_t currently belonging to the ActionProviderPlugin...

	/**Temporary to meet legal requirements.
	Open the credit screen overlay.*/
	void toggleCredits();

private:
	/**Available info screens (do not need a full object-oriented stragegy-like solution: is just 2 screen.
	May refactor if more screens are introduced - unlikely by now).*/
	enum infoScreens {rotationRateScreen, 
					  statusScreen
					};

	/**What is currently displayed.*/
	infoScreens currentScreen;

	/**Status of the device (working/broken)*/
	bool broken;

	/**Shortcut to order the display of roll rates - could be done with the raw input, but makes
	more sense to deal with the formatting (and eventual indicator light to be turned on) here.*/
	void rollRates();

	/**Format and display the ship stats (health, by now).*/
	void stats();

	/**Master alarm on (true) or off (false).*/
	bool masterAlarmActive;

	/**Master alarm blinking state, true=light and false=no light.*/
	bool masterAlarmBlink;

	/**Reads the data about the spaceship from this struct. It MUST be initialized before any
	use of the data (but cannot be a reference, as it is owned/created by the spaceship and "this" class does
	not know the SpaceShip internals...)*/
	SpaceShipStatus* dataBuffer;

	///Knows how to create all the visual elements in the cockpit and toggle their settings. OWNED by this class.
	CockpitScreenElements* screenElements;

	///Counts the radar dots.
	IDGenerator radarDotsIDMaker;
};

