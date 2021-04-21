#pragma once

#include <string>

///Type of the numeric IDs.
typedef unsigned long long int numeric_id_t; //The biggest int I can think of. Not portable. Should we make a class and encapsulate?

class IDGenerator
{
	/**There are a few places where unique names are required. E.g. Ogre objects, sensor contacts...
	This class is a utility to generate them: just counting the objects. They can be either numeric_id_t or std::strings.
	Different instances of IDGenerator will generate the same IDs. It is up to the client to avoid mix-ups.
	There is no provision to avoid the overflow of the internal counter - we don't expect to have that many objects!*/
public:
	/**Gives a generator that justo count objects. It will give numeric IDs (1, 2, 3...).
	String IDs do not have prefixes ("1", "2", "3"...).*/
	IDGenerator(void);
	
	/**Set a specific prefix in the generator to give string ID with a prefix ("idPrefix1", "idPrefix2", "idPrefix3"...).
	Numeric IDs are available as usual.*/
	IDGenerator(const std::string& idPrefix);

	/**Returns a numeric ID.*/
	numeric_id_t getNewNumericId();

	/**Returns a string ID.*/
	std::string getNewStringId();

private:
	///Common initialization code for constructors.
	void init();

	///Prefix to generate string IDs.
	std::string idPrefix;

	///Internal counter of IDs.
	numeric_id_t counter;
};

