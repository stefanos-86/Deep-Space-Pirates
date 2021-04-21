#include "StdAfx.h"
#include "VisualDebuggerConnector.h"

#include <exception>

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>	

#ifdef _DEBUG //Will be a empty file if not in debug mode

VisualDebuggerConnector::VisualDebuggerConnector(hkpWorld* havokWorld){
	context = new hkpPhysicsContext();
	hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
	context->addWorld(havokWorld); // add the physics world so the viewers can see it
	havokWorld->m_wantDeactivation = false;
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(context);
	vdb = new hkVisualDebugger(contexts);
	vdb->serve();
}


VisualDebuggerConnector::~VisualDebuggerConnector(void){
	vdb->removeReference();
	context->removeReference();
}

void VisualDebuggerConnector::step(){
	vdb->step();
}

#endif
