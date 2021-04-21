#pragma once
#include "stdafx.h"

//class ActionName; TODO (?) This line gives a compilation error, but then how does this module know the class?
#include "ActionName.h"


/** This is the interface for the action providers that react to input.
Notice that every provider must react to keyboard input (pure virtual functions), but mouse listening is optional.
If in the future we have "mouse-only" commands, then we can make the keyboard stuff optional too.*/
class ActionProviderPlugin
{
public:

	/**This type represent the mouse coordinates.*/
	//TODO: this may change in the future, depending on which options are available for the mouse.
	class MousePosition {
		public:
			MousePosition() :
				xPixels(0),
				yPixels(0),
				xNorm(0),
				yNorm(0),
				wheelSteps(0) {}

			//Mouse coordinates in pixels. This is the reference system for screen objects and the like.
			int xPixels; //horizontal displacement
			int yPixels; //vertical
			//double deltas ? May be added later.
			//Mouse coordinates normalized in [-1,+1] (for pixel-independent jobs). (0,0) is the center of the screen.
			double xNorm; //-1 = left border of the window.
			double yNorm; //-1 = top of the window
	
			/**The mouse wheel value given by the input libraries seems unlimited, therefore we store
			the delta. This class allows the rest of the system to know the direction of the last wheel
			movement and the number of steps. The sign gives the direction (+ is ahead of the mouse).
			The unit are the steps.*/
			int wheelSteps;
	} ;

	/**This is the only thing to call to execute an action.
	@param action the name of the action to do
	@param stop true if it is a key up (needed if a duration action is terminating)*/
	virtual void doAction(ActionName const& action, bool stop) = 0;

	/**Returns a vector of the names of the actions that the executor can do.
	The very same strings must be used for the key binding in the configuration file.*/
	virtual std::vector<ActionName> declareActions() = 0;

	/**Returns true if the plugin has to be registered for mouse positions updates.
	Mouse buttons are considered buttons, thus are linked to actions just as the keyboard keys: listening
	to the mouse is not required. Default to false.*/
	virtual bool isMouseListener() const;

	/**Called at any cycle to give the current mouse position to a plugin that is registered to listen
	to mouse movements. Default to a failed assertion (makes no sense to listen to the mouse and then use the "do nothing"
	implementation, must be a programming error).*/
	virtual void mouseMove(const MousePosition& currentMousePosition);

};
