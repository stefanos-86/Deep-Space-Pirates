/** @file
This file should contain only the "main"*/
#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <exception>
// Logging
#include "Logger.h"
//Technical systems.
#include "ConsoleOutput.h"
#include "SanityCheck.h"
//Game subsystems.
#include "goManager.h"
#include "InputMapper.h"
#include "HavokFacade.h"
#include "SystemClock.h"
#include "OgreFacade.h"
//Game loop
#include "GameLoop.h"

/********INCLUDES BELOW THERE ARE EXPECTED TO BE REFACTORED SOON OR LATER
	they may have logic that should not go in a main... ************/
//audio includes
#include "FMODfacade.h"
#include "dsSound.h"

//Camera system.
#include "Cameraman.h"

//to test the input mapper the key codes are needed
#include <OISKeyboard.h>
#include "NoOpCommand.h"

#include "SystemCommands.h" //action provider for system-wide actions

//this should be temporary///////////////
#include "dsMath.h"
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <winbase.h>

#include "SensorBus.h"
#include "FixedRadarTarget.h"

#include "EffectsLibrary.h"

////////////////////////////////////////

//Game items builders
#include "SpaceDebrisCreator.h"
#include "SpaceshipCreator.h"

//////
//Leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
///////////


/**
Project entry point.
Also provides a "mock up" initialization to launch the game (and do more tests).
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{	
	//Attempt to collect all and every excetpion for logging.
	//Do this only in release mode - in debug mode you want visual studio to open the debugger.
	#ifndef _DEBUG
	try{
	#endif


	//activate debug console I/O
	#ifdef _DEBUG
		RedirectIOToConsole();
	#endif

	Logger::InitLogging(strCmdLine);

	LOG(INFO) << "Before sanity check.";

	/*Before any activity, verify the system status*/
	SanityCheck sc;
	if(! sc.runTests()){
		std::cout<<"Something wrong with environment, terminating."<<std::endl;
		LOG(ERROR) << "Something wrong with environment, terminating...";
		system("pause");
		exit(0);
	}else{
		LOG(INFO) << "Environment is sound, starting";
	}
	
/**Game engine machinery initialization**/

	HavokFacade::getInstance(); //Ensure that we call the constructor...
								//...before creating havok stuff using "new" - havok has its special allocation.

	LOG(INFO) << "Havok started.";

	//This must be created before the spaceship for the cockpit to use it...
	OgreFacade *of = OgreFacade::getInstance();
	LOG(INFO) << "Ogre prepared.";
	InputMapper im;
	LOG(INFO) << "Input mapper ready started.";
	of->init(&im);
	LOG(INFO) << "Ogre started.";


	SystemClock::getInstance()->addListener(HavokFacade::getInstance());

	//General sub system for sensors.
	//Must be created very early, to permit the registration of the targets.
	SensorBus* s = new SensorBus();
	GameObject* radar = new GameObject("SensorBus");
	radar->addComponent(s);
	goManager::getInstance()->addGameObject(*radar);

/*************CODE TO SHOW SOMETHING ON SCREEN AND TEST THE MACHINERY***************/
	//we will load data here... By now, it is hardcoded
		
	//Some test objects to try out collisions and get a reference in space.
	SpaceDebrisCreator debrisCreator;
	std::vector<std::string> collisionTargetNames;
	debrisCreator.createCollisionTarget("testTarget", -50, 0, 0);
	collisionTargetNames.push_back("testTarget");
	for(int i=1; i<5; i++)
	for(int j=1; j<5; j++){
			char name[]="Crash I J";
			sprintf(name,"Crash %d %d", i, j);
			debrisCreator.createCollisionTarget(name,40, j*10-25,  i*10 - 25);
			collisionTargetNames.push_back(name);
		}
	debrisCreator.createCollisionTarget("gunTestTarget", 30, 0, 0);
	collisionTargetNames.push_back("gunTestTarget");
	 
//	debrisCreator.createReferenceAxes();
//	debrisCreator.createReferenceCompass();

	//The demo particle effect.
	Vector3d position(15,0,0);
	OgreFacade::getInstance()->activateEffect("Grey smoke puff", position);

	//The player spaceship.
	SpaceshipCreator shipyard;
	shipyard.createPlayerSpaceship(im);

	
	//Audio.
	FMODfacade* ff = FMODfacade::getInstance();
	//BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!BAD!!!
	//Path in release is different!
	dsSound *backgroundMusic = ff->getLoopSound("..\\..\\media\\dsp\\audio\\back_music.mp3");
	ff->play(*backgroundMusic);
		
	//Camera (will auto-register to the facade).
	//TODO: make a full GOC out of it? In order to register in the goManager and to split logic/commands/video... in more classes?
	//Must be started after the player spaceship is registered to the ogre Facade in order to attach cameras to node (makes sense: first create the
	//scene, then place the cameras).
	Cameraman camera;

	//The game loop
	GameLoop game(*of);
	
	//add the system-wide commands input plugin
	SystemCommands* sysCommandPlugin= new SystemCommands(&im, &game);
	im.registerActionsProvider(sysCommandPlugin);
	im.registerActionKey(SystemCommands::SHUTDOWN, OIS::KC_ESCAPE);


	//TEST: a fake action executor registration - disabled: key T is needed elsewere...
	//im.registerActionKey("TEST NO OP ACTION", OIS::KC_T);
	//im.registerActionsProvider(new NoOpCommand());

	//Regiter the other command providers.
	im.registerActionsProvider(&camera);
	
/*************END OF CODE TO SHOW SOMETHING ON SCREEN AND TEST THE MACHINERY***************/

	//Begin drawing: start ogre.
	try{
		of->fireUp(); //start ogre
	}catch(std::exception e){
		//no recover possible
		LOG(ERROR) << "Could not start ogre: exception raised.";
		LOG(ERROR) << e.what();
		//DSLOG("Could not start ogre: exception raised.");
		//DSLOG(e.what());
	}

	//Can now start the game;
	game.start();

/*************Out of game loop: clean up steps.***************/
    //Cleanup of objects dependant on the facades:
	debrisCreator.dismantleCollisionTarget(collisionTargetNames);

	//cleanup: erasing singletons
	goManager::destroy(); //Keep as first: we take care of deleting some havok objects. If we dont havok may kill them itself - and when we retry later, dangling pointers!
	SystemClock::destroy();
	FMODfacade::destroy();
	HavokFacade::destroy();
	OgreFacade::destroy();
	
	//and the configuration store? And the goFactory?

	//Cleanup: other objects.
	delete backgroundMusic;

	//stop to allow to read output
	#ifdef _DEBUG
		//system("pause");
	#endif

	//Check for leftovers. 
	//_CrtDumpMemoryLeaks();  //Current output of this after "Dumping objects ->" is 3240 lines long.
	

	//Catch and log exceptions, but only in release.
	//In debug mode you are supposed to have the debugger intercept them.
	#ifndef _DEBUG
	}catch (std::exception& e){
		LOG(ERROR) << e.what();
	}
	catch (...){
		LOG(ERROR) << "Unknown exception...";
	}
	#endif

	return 0;
}

//*****************Main Page Doxigen Stuff***************

/*! \mainpage General informations
 *
 * \section intro_sec Introduction
 *
 * Aim of this project is to learn how to program games.
 When you modify it, remember not to use settings local to your environment.

 *
 * \section install_sec How to compile and run.
 *
See a detailed guide at http://sourceforge.net/apps/mediawiki/dspirates/index.php?title=Build_From_Nothing.
 */
 
