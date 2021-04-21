#include "StdAfx.h"
#include "FMODfacade.h"
#include <stdexcept>

SINGLETON_DEF(FMODfacade);

FMODfacade::FMODfacade(){
	//most of this code comes from a FMOD tutorial that... I can't find anymore on the web!
	unsigned int version;
	int numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	char name[256];

	/*Create a System object and initialize it.*/
	result = FMOD::System_Create(&system);
	errorCheck();
	result = system->getVersion(&version);
	errorCheck();

	/*version check*/
	if (version < FMOD_VERSION) {
		//DSLOG("Error! You are using an old version of FMOD");
		LOG(ERROR) << "Error! You are using an old version of FMOD";
		//printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
		throw std::runtime_error("old FMOD version");
	}

	/*detect audio drivers*/
	result = system->getNumDrivers(&numdrivers);
	errorCheck();
	if (numdrivers == 0){ //no audio
			result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
			//DSLOG("No drivers detected No sound!!!");
			LOG(WARNING) << "No drivers detected No sound!!!";
	}else{
			result = system->getDriverCaps(0, &caps, 0, 0, &speakermode);
			/*Set the user selected speaker mode - but don't know where the user selects them.*/
			result = system->setSpeakerMode(speakermode);
			errorCheck();
			//DSLOG("Audio drivers found");
			LOG(INFO) << "Audio drivers found";

			if (caps & FMOD_CAPS_HARDWARE_EMULATED){
				/*The user has the 'Acceleration' slider set to off! This is really bad for latency! You might want to warn the user about this.*/
				result = system->setDSPBufferSize(1024, 10);
				errorCheck();
				//DSLOG("Audio drivers not accelerated");
				LOG(INFO) << "Audio drivers not accelerated";
			}

			/*Special cases for special drivers*/
			result = system->getDriverInfo(0, name, 256, 0);
			errorCheck();
			if (strstr(name, "SigmaTel")){
				//DSLOG("SigmaTel audio driver");
				LOG(INFO) << "SigmaTel audio driver";
				/* Sigmatel sound devices crackle for some reason if the format is PCM 16bit. PCM floating point output seems to solve it. */
				result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			}
	}

	/*complete system init*/
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	errorCheck();
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER){
		//DSLOG("Speaker mode not supported by soundcard, reverting to stereo");
		LOG(WARNING) << "Speaker mode not supported by soundcard, reverting to stereo";
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		/*... and re-init. */
		result = system->init(100, FMOD_INIT_NORMAL, 0);
	}


}


FMODfacade::~FMODfacade(){
	try{
		result = system->release();
		errorCheck();
	} catch(std::exception e){
		//RLOG("FMOD facade destruction failed");
		//RLOG(e.what());
		LOG(ERROR) << "FMOD facade destruction failed";
		LOG(ERROR) << e.what();
	}
}



void FMODfacade::errorCheck(){
	if (this->result != FMOD_OK) {
		//DSLOG("---FMOD error---");
		//DSLOG( FMOD_ErrorString(this->result));
		//DSLOG("----------------");
		LOG(ERROR) << "---FMOD error---\n" << FMOD_ErrorString(this->result) << "\n" << "----------------";
		throw std::runtime_error("FMOD error");
    }
}

dsSound* FMODfacade::getSound(std::string audioFileName){
	dsSound* ret = new dsSound();	  
	result = system->createSound(audioFileName.c_str(), FMOD_SOFTWARE | FMOD_CREATESTREAM, &(ret->soundExInfo), &(ret->sound));
	errorCheck();
	return ret;
}

dsSound* FMODfacade::getLoopSound(std::string audioFileName){
	dsSound* ret = getSound(audioFileName);
	result = ret->sound->setMode(FMOD_LOOP_NORMAL);
	errorCheck();
	return ret;
}

void FMODfacade::play(dsSound &clip){
	result = system->playSound(FMOD_CHANNEL_FREE, clip.sound, 0, &(clip.channel));
	errorCheck();
}

void FMODfacade::stop(dsSound &clip){
	clip.channel->setPaused(true);
}