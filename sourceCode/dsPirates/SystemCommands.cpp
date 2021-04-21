#include "StdAfx.h"
#include "SystemCommands.h"
#include "OgreFacade.h"
#include "InputMapper.h"
#include "GameLoop.h"

/*actual strings for actions*/
const ActionName SystemCommands::SHUTDOWN="close";

SystemCommands::SystemCommands(InputMapper* im, GameLoop* gl):
	im(im),
	gl(gl)
{ }


void SystemCommands::doAction(ActionName const& action, bool stop){
	
	if(action == SystemCommands::SHUTDOWN){
		//Main loop.
		this->gl->stop();

		/*shutdown the subsystems*/
		//detach input
		this->im->unregisterFromInput();
		
		//ogre
		OgreFacade::getInstance()->endAll();

		//AUDIO??FMOD?? No clues about any action required to close it.

		//DSLOG("(System commands) Game closed");
		LOG(INFO) << "(System commands) Game closed";
		return; //all system closed, do not evaluate any further
	}
	
	//if the flow gets there, then the action is unknown (not provided by this module)
	//DSLOG("(System commands) Unknown action.");
	LOG(WARNING) << "(System commands) Unknown action.";
	return; //do nothing if action is unknown

}

std::vector<ActionName> SystemCommands::declareActions(){
	std::vector<ActionName> ret;

	ret.push_back(SystemCommands::SHUTDOWN);

	return ret;
}
