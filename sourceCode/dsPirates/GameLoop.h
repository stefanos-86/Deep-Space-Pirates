#pragma once

class OgreFacade;

/** Here is the main loop. It goes on forever until the shutdown command (or 21/12/2012, whichever comes first).
Before calling it, everything must be up and running.
*/
class GameLoop
{
public:
	/**Begin the loop*/
	void start();
	/**End the loop*/
	void stop();

	void pause();

	GameLoop(OgreFacade& of);
private:
	///When set to false, stops the loop.
	bool running;
	///Must be called to render frames.
	OgreFacade& ogre;
	
	bool paused;
};

