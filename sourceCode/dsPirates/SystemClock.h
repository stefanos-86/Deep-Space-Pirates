#pragma once

#include "EasySingleton.h"
/**Central point of synchronization in the system.
Observer pattern: all the elements that need to "advance in time" are notified
of the elapsed time by this service. They are called once per each game loop.

The game loop calls this singleton and passes the time to propagate.
All clients should implement the TimedElement interface.
*/

//List of registered elements.
#include<vector>
#include "IDGenerator.h" //Timers IDs.

class TimedElement;

class SystemClock
{
	SINGLETON(SystemClock);

public:
	/**The duration is expressed in the unit of the caller and just passed along.
	The game loop uses seconds. The callback used to notify each timed element is timeStep.*/
	void propagateTime(float duration);
	void addListener(TimedElement* listener);

	/**A timer for the listener is set to the desired duration and started immediatly. Once it goes to 0 or below
	the timedCallback of the client is called. The function returns the id of the callback (if the client wants to 
	register to more than a callback). If repeat is false, no further calls are done. Otherwise the timer is reset and
	restarted immediately.
	Notice that the listener is actually added during prpoagateTime, that is, at the next game loop.*/
	numeric_id_t addDurationListener(TimedElement* listener, float targetDuration, bool repeat);

	/**Unregister all the duration listeners of the timed element.
	The timers are queued for deletion (to avoid messing up the vector of timers during the iteration on it)
	and actually deleted at the next game loop.
	*/
	void deleteAllDurationTimers(TimedElement* listener);

	/**Unregister the listener from the propagate timer loop.
	Keeps the duration timing.*/
	void removeListener(TimedElement* listener);
	
private:
	/**Contains pointers to the objects that are to be notified of the time passing.
	This subsistem does NOT own the object - won't delete them.*/
	std::vector<TimedElement*> listeners;

	/**Special kind of listener that is called only when the duration expires rather than at every cycle.*/
	typedef struct {
		TimedElement* listener;
		float targetDuration;
		float timeAccumulator;
		numeric_id_t id;
		bool repeat;
		bool pendingDeletion;  //Some listeners may ask to stop receivin updates when they are called. To avoid a mess up with iterators they mark themseves for removal. Acutal removal is at the next cycle.
	} DurationTimer;

	std::vector<DurationTimer> durationListeners;
	/**Some listener may create other listeners during the callback (e.g. the spaceship creates bullets).
	The new listeners are parked here so that durationListeners is not altered during the iterations. */
	std::vector<DurationTimer> durationListenersPendingRegistration;

	//Support functions to manipulate DurationTimers (see comments in implementations)
	static bool ListenerMatch(TimedElement* listener, const DurationTimer& timer);
	static bool ExpiredTimer(const DurationTimer& timer);
	static bool PendingDeletion(const DurationTimer& timer);

	///Helper object to create IDs.
	IDGenerator timerIDMaker;
};

