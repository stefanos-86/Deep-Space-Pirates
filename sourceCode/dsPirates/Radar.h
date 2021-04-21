#pragma once

/**This class is part of the spaceship logic. It deals with simulation of the radar,
up to the printing of dots on a screen (handled by the instrument panel).*/

class Cockpit;
class gocVisualModel;

class Radar
{
public:
	/**
	@param screen the cockpit object that can actually draw the dots on the screen
	@param the maximum distance that the radar can cover (in meters, that is WC units)
	@param forCoordinates the visual object that draws the spaceship - to re-use the graphic engine geometry libs.*/
	Radar(Cockpit* screen, double range, gocVisualModel* forCoordinates);
	~Radar(void);

	/*Causes the radar to look for contacts and update the screen.
	*/
	void scan();

private:
	///Holds a reference to the radar screen. Not owned.
	Cockpit* screen;

	///Square of the maximum range of the radar un meters.
	//The vector library has a squaredLength function that is more efficient than length.
	double squaredRange;

	///Used as a helper to change coordinates of objects between local and WC.
	gocVisualModel* coordinates;
};

