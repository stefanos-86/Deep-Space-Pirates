#pragma once
#include "gocaudio.h"

class dsSound;

/**This class deals with the audio of the spaceship. Engine noises, crashing into things, alarms...*/
class ShipSound :
	public gocAudio
{

public:
	/**Pre-load the sound clips. The file names are hardcoded here.*/
	ShipSound(void);
	~ShipSound(void);

	///Starts the "beep beep" from the alarm.
	void startMasterAlarm();

	///"Sdong"- sound for impacts.
	void collisionSound();

	/**"Bang" - sound for gun shots.
	It is not convenient to implement this as a loop.
	In case of single shots the loop can't be stopped just when the fire key is raised.
	We must wait at least a cicle. There may be a FMOD option to do this with loops.
	Even then, we would have to keep the loop "in synch" with the gun, or adjust the frequency of fire to
	the loop duration. Cumbersome.
	*/
	void gunShot();

private:
	
	/**Audio clips for various effects (pre-loaded at class creation).
	This objects OWNS ALL of them.*/

	dsSound* masterAlarm;
	dsSound* collision;
	dsSound* gun;
};

