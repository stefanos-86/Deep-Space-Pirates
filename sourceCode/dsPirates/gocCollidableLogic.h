#pragma once
#include "goclogicbase.h"
#include "Physics/Dynamics/Collide/ContactListener/hkpContactListener.h"
#include <string>

/**This is the common superclass for all the legics of all the game object that may collide with each other in the
game world. It ensure that the dynamic casts for the double dispatching is successful (we are taking about this:
http://en.wikipedia.org/wiki/Multimethods#C++ - this class has the role of Thing).

This class also "pulls in" the inheritance from hkpContactListener that is required to notify objects of the collisions
and provides a "empty" implementation that does nothing.

*/
class gocCollidableLogic :	public gocLogicBase, public hkpContactListener
{
public:
	gocCollidableLogic(const std::string& name);
	~gocCollidableLogic(void);

	virtual void contactPointCallback( const hkpContactPointEvent& event );
};

