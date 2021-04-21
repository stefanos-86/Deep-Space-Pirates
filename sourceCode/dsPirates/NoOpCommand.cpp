#include "StdAfx.h"
#include "NoOpCommand.h"

void NoOpCommand::doAction(ActionName const& action, bool stop){
	//DSLOG("----------------------");
	//DSLOG("No-op test executor");
	//DSLOG(action);
	LOG(INFO) << "----------------------\n" << "No-op test executor\n" << action;
	if(stop)
	//	DSLOG("   terminated");
	    LOG(INFO) << "   terminated";
	//DSLOG("----------------------");
}

std::vector<ActionName> NoOpCommand::declareActions(){
	std::vector<ActionName> ret;
	ret.push_back("TEST NO OP ACTION");
	return ret;
}
