#include "StdAfx.h"

#include "SystemClock.h"
#include "EasySingleton.h"
#include "TimedElement.h"


SINGLETON_DEF(SystemClock); 


void SystemClock::propagateTime(float duration){
	//Propagate the timing to normal listeners.
	for (std::vector<TimedElement*>::iterator it = this->listeners.begin();
		it != this->listeners.end(); ++it){
			(*it)->timeStep(duration);
	}
	//Remove any listener that was marked for removal/parked as pending destruction at the previous loop.
	this->durationListeners.erase(
		std::remove_if(this->durationListeners.begin(), this->durationListeners.end(), PendingDeletion),
		this->durationListeners.end()
	);

	//Advance the timers and call them if needed.
	for (std::vector<DurationTimer>::iterator i = this->durationListeners.begin();
		i != this->durationListeners.end(); ++i){
			i->timeAccumulator += duration;  //step the timer
			if (i->timeAccumulator>=i->targetDuration){	//Time to call and reset)
				i->listener->timedCallback(i->id, i->timeAccumulator);
				if(i->repeat==true){ //Reset the repeatable timers.
					i->timeAccumulator=0;
				}
			}
	}

	//Clean the expired timers.
	this->durationListeners.erase(
	   std::remove_if(this->durationListeners.begin(), this->durationListeners.end(), ExpiredTimer),
	   this->durationListeners.end()
	);

	//Add anything that has been created during the iteration.
	std::copy(this->durationListenersPendingRegistration.begin(),
			  this->durationListenersPendingRegistration.end(),
			  std::back_inserter(this->durationListeners));
	this->durationListenersPendingRegistration.clear();
}

void SystemClock::addListener(TimedElement* listener){
	this->listeners.push_back(listener);
}


numeric_id_t SystemClock::addDurationListener(TimedElement* listener, float targetDuration, bool repeat)
{
	DurationTimer newListener;
	newListener.targetDuration=targetDuration;
	newListener.timeAccumulator=0;
	newListener.id= this->timerIDMaker.getNewNumericId();
	newListener.listener=listener;
	newListener.repeat=repeat;
	newListener.pendingDeletion=false;
	this->durationListenersPendingRegistration.push_back(newListener);
	return newListener.id;
}

void SystemClock::deleteAllDurationTimers(TimedElement* listener)
{
	//Scan the listers to find which are to be deleted.
	std::vector<DurationTimer>::iterator candidate;
	for (candidate=this->durationListeners.begin(); candidate!=this->durationListeners.end(); ++candidate)
		if (ListenerMatch(listener, *candidate)) //found one, mark for removal
			candidate->pendingDeletion=true;
}

//Returns true if the listener in the timer is the expected one. Supports remove_if.
bool SystemClock::ListenerMatch(TimedElement* listener, const DurationTimer& timer){
	return timer.listener == listener;
}

//Returns true if the timer is marked as pending deletion
bool SystemClock::PendingDeletion(const DurationTimer& timer){
	return timer.pendingDeletion;
}

//Returns true if the timer is useles (all the time has passed and no repetition is expected).
bool SystemClock::ExpiredTimer(const DurationTimer& timer){
	return timer.repeat==false && timer.timeAccumulator>=timer.targetDuration;
}

void SystemClock::removeListener(TimedElement* listener){
	std::vector<TimedElement*>::iterator removeThis = std::find(this->listeners.begin(), this->listeners.end(), listener);
	this->listeners.erase(removeThis);
}