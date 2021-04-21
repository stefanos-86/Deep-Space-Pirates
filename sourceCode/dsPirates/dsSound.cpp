#include "StdAfx.h"
#include "dsSound.h"

#include "FMODfacade.h"

dsSound::dsSound(){
	/*Allocate memory for an instance of FMOD_CREATESOUNDEXINFO
	This must be done to initialize the FMOD_CREATESOUNDEXINFO before playback
	Is it done here (instead of in the facade) as it is little more than memory management*/
    memset(&soundExInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    soundExInfo.cbsize   = sizeof(FMOD_CREATESOUNDEXINFO);
   
	/*The actual sound initialization is done by the FMOD facade, as it is also
	the factory for those kind of object.*/
}

