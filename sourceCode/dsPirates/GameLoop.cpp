#include "StdAfx.h"

#include "GameLoop.h"
#include "OgreFacade.h"
#include "SystemClock.h"
#include <iostream>

GameLoop::GameLoop(OgreFacade& of) :
	ogre(of),
	paused(false)
{
	
	//not starting yet
	this->running=false;
}

void GameLoop::start(){
	this->running=true;

	while(running){
		//Draw a frame and get the stopwatch.
		unsigned long elapsedTime = this->ogre.stepFrame();
		//Drive the system clock to step the simulation.
		if(!this->paused)
			SystemClock::getInstance()->propagateTime(elapsedTime * 0.000001f); //convert to seconds
	}
}

void GameLoop::stop(){
	this->running=false;
}

void GameLoop::pause(){
	if (this->running && !this->paused)
		this->paused = true;
	else
		this->paused=false;
}


