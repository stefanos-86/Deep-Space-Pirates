
#pragma once
#include "ActionProviderPlugin.h"

/**Test class. This executor just log the action name, and the "start/stop" conditions.*/

class NoOpCommand :
	public ActionProviderPlugin
{
public:
	virtual void doAction(ActionName const& action, bool stop);
	virtual std::vector<ActionName> declareActions();

};

