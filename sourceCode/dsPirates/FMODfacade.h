#include "stdafx.h"
#pragma once
#include "EasySingleton.h"


#include "fmod_errors.h" //errcheck
#include "fmod.h" 
#include "fmod.hpp" //FMOD namespace

#include "dsSound.h"

/**This class is the entry point to the FMOD sound engine. It supports the sound components.
It requires external dependencies.

fmodex_vc.lib;
FMOD_HOME (example) = C:\Program Files (x86)\FMOD SoundSystem\FMOD Programmers API Win32\api
Include from $(FMOD_HOME)\inc
Include from $(FMOD_HOME)\lib 

*/

class FMODfacade { 
	SINGLETON(FMODfacade);

public:
	/**This constructor initializes the FMOD machinery.
	@throws runtime_error if initialization fails*/
	FMODfacade();

	/**This destructor deallocates FMOD.
	Try-catches exceptions if destruction of FMOD fails. Log the problem, but can not recover.*/
	~FMODfacade();

	/**Create a playable sound object given the file name of the audio data (.mp3, .wav, .mid...).
	The client is expected to delete the object!
	@throws runtime_error if any error is detected
	@returns a dsSound object that can be played
	@param the file name of the audio clip (please give a FULL PATH!)*/
	dsSound* getSound(std::string audioFileName);

	/**Same as getSound, but the sound object will play in loops.
	The client is expected to delete the object!
	@throws runtime_error if any error is detected
	@returns a dsSound object that can be played
	@param the file name of the audio clip (please give a FULL PATH!)*/
	dsSound* getLoopSound(std::string audioFileName);

	/**Start to play the sound.
	@throws runtime_error if any error is detected
	*/
	void play(dsSound &clip);

	/**Stop the sound.
	It actually just pauses it, but I don't know how to "fully stop" a FMOD object. TODO: find the proper way to do this.
	@throws runtime_error if any error is detected
	*/
	void stop(dsSound &clip);

private:
	/**This function contains the error-reaction code to check FMOD operations.
	It logs the the error and throws an exception; if all OK it just terminates.
	In case no audio drivers are found it will default to the "no sound" settings.
	@throws runtime_error if any error is detected
	*/
	void errorCheck();

	
	/**FMOD entry point*/
	FMOD::System *system;
	/**Last error code from FMOD. The programmer must save it here at every FMOD call and then call the error check,*/
	FMOD_RESULT result;


};

