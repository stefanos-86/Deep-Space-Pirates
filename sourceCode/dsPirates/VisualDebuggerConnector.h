#ifdef _DEBUG //Will be a empty header if not in debug mode.
#pragma once

class hkVisualDebugger;
class hkpPhysicsContext;
class hkpWorld;

/**This class encapsulate the logic for the HavokVisualDebugger.
Can be used only in debug mode.*/
class VisualDebuggerConnector
{
public:
	VisualDebuggerConnector(hkpWorld* havokWorld);
	~VisualDebuggerConnector(void);
	void step();
private:
	hkVisualDebugger* vdb;
	hkpPhysicsContext* context;
};


#endif