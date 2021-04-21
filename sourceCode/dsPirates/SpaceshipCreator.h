#pragma once

class InputMapper;

/**This class provides all the logic to create spaceship in the game world (BY NOW ONLY THE PLAYER ONE)
by assembling the needed components and registering them to the various facades.*/
class SpaceshipCreator
{
public:
	void createPlayerSpaceship(InputMapper& inputMapper);
};

