#include "StdAfx.h"
#include "EffectsLibrary.h"
#include "Ogre.h"
#include "SystemClock.h" //To destroy the particle effects when done.

EffectsLibrary::EffectsLibrary(Ogre::SceneManager* sceneManager) :
	particleNameMaker("particleSystem"), //Fixed prefix ID.
	sceneManager(sceneManager)
{
	this->name2template["Grey smoke puff"] = "basicGreySmoke";
}

void EffectsLibrary::startEffect(const std::string& name, const Vector3d& position){
	//Find the implementation.
	std::map<std::string, std::string>::const_iterator implementationTemplate = this->name2template.find(name);
	if (implementationTemplate == this->name2template.end()){ //Throw if unexistent effect..
		//DSLOG("Unknown effect");
		//DSLOG(name);		
		LOG(WARNING) << "Unknown effect";
		LOG(WARNING) << name;
		throw std::runtime_error("Programming error! Unknown effect requested (more in logs)."); //Notice that when thrown the debugger halts on the NEXT line.
	}
	std::string effectSystemName=this->particleNameMaker.getNewStringId();
	Ogre::ParticleSystem* particleSystem = this->sceneManager->createParticleSystem(effectSystemName, implementationTemplate->second);
	//Implementation restriction: only 1 emitter, for simplicity.
	if(particleSystem->getNumEmitters() != 1){
		const std::string errorMessage("Particle scripting error. 0 emitters, or more than 1.");
		//DSLOG(errorMessage);
		LOG(ERROR) << errorMessage;
		LOG(FATAL) << "ciao";
		throw std::runtime_error(errorMessage);
	}
	particleSystem->getEmitter(0)->setPosition(position);
	this->sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(particleSystem);

	///Register a deletion timer.
	double totalDuration = particleSystem->getEmitter(0)->getDuration()+particleSystem->getEmitter(0)->getTimeToLive();
	numeric_id_t deletionTimer=SystemClock::getInstance()->addDurationListener(this, totalDuration, false);
	this->timerId2EffectName[deletionTimer]=effectSystemName;  //So that we can pick up the right effect in the time callback.
}

void EffectsLibrary::timedCallback(numeric_id_t id, float actualDuration){
	this->sceneManager->destroyParticleSystem(this->timerId2EffectName[id]);
}