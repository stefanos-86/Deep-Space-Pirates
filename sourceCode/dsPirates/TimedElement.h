#pragma once

#include "IDGenerator.h" //Type for IDs.

/**This interface "flags" object that can update their status as time passes, with a call to the time stepping method.
Those objects are called by the SystemClock, that also passes the elapsed time since the last call (more or less...).*/
class TimedElement
{
public:
	/**The system clock notifies the timed element that a game loop "passed" qnd provides the duration.*/
	virtual void timeStep(float stepDuration) {};

	/**The system clock notifies the timed element that the timer "id" has gone to 0.
	The actual duration compensate the fact that the duration of game loops may not add up exaclty to the client-desired duration.
	As soon as the duration is exceeded the time is reported.*/
	virtual void timedCallback(numeric_id_t id, float actualDuration) {};
};


