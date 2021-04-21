#pragma once

#include "fmod.h" 
#include "fmod.hpp" //FMOD namespace

/**This class represents a sound effect: a audio file that can be played.
It stores the needed FMOD data.
(it is dsSound and not just Sound due to a name clash)*/
class dsSound
{
private: 
	/**Private constructor: only the facade can give sound objects.*/
	dsSound();
	FMOD_CREATESOUNDEXINFO soundExInfo;

	/*FMOD sound representation.*/
	FMOD::Sound *sound;
	/*This sound will play on this channel*/
    FMOD::Channel *channel;

	/**FMODfacade class is the factory for dsSound objects: it needs to work with them and the dsSound objects have no
	setters for the FMOD-related data to preserve the information hiding*/
	friend class FMODfacade;

};

