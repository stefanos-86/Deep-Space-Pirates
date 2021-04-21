#pragma once

#include "actionproviderplugin.h"
#include "ActionName.h"

class InputMapper;
class GameLoop;

class SystemCommands  : public ActionProviderPlugin
{
public:
	SystemCommands(InputMapper* im, GameLoop* gl);

	//satisfy the input provider interface
	virtual void doAction(ActionName const& action, bool stop);
	virtual std::vector<ActionName> declareActions();

	/**This action closes the game*/
	static const ActionName SHUTDOWN;


private:
	/*The input mapper requires to take action at shutdown, and a reference to objects to be controlled
	must be available.*/
	InputMapper* im;
	GameLoop* gl;
};



