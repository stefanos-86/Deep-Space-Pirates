#pragma once

#include "gocLogic.h"

/**This is the logic component for objects with... no logic at all.
It just stores the data needed to the rest of the system to handle the object.
(currently, the name).
.*/
class gocLogicBase : public gocLogic
{
public:
	gocLogicBase(const std::string& name);
	virtual ~gocLogicBase() {};

	/**Returns the type of the component.
	@returns the object type*/
	//virtual const goc_id_type componentID() const; - will not overryde this by now. We need a naming convention and a place to store the strings...

	const std::string getObjectName();

private:
	/**This string identifies the owner of this component in the system.*/
	std::string objectName;
};

