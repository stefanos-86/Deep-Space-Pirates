#include "stdafx.h"
#include "InputMapper.h"
#include "ConfigurationStore.h"
#include "ActionProviderPlugin.h"
#include "ActionName.h"

//included for test reasons only (ifdef DEBUG??)
#include "NoOpCommand.h"

using namespace OIS;


/**Inizialize on first use the vector of known key names.
This key names must be used in the configuration files.*/
const std::map<std::string, OIS::KeyCode>& knownKeyNames(){
	static std::map<std::string, OIS::KeyCode> ret; //return value;
	static bool ready=false;
	
	if(ready==false){
		ret["C"]=OIS::KC_C;
		ret["X"]=OIS::KC_X;
		ret["A"]=OIS::KC_A;
		ret["D"]=OIS::KC_D;
		ret["W"]=OIS::KC_W;
		ret["S"]=OIS::KC_S;
		ret["L"]=OIS::KC_L;
		ret["K"]=OIS::KC_K;
		ret["U"]=OIS::KC_U;
		ret["J"]=OIS::KC_J;
		ret["I"]=OIS::KC_I;
		ret["O"]=OIS::KC_O;

		ret["N"]=OIS::KC_N;
		ret["M"]=OIS::KC_M;

		ret["T"]=OIS::KC_T;

		ret["SPACEBAR"]=OIS::KC_SPACE;
		
		ret["F1"]=OIS::KC_F1;
		ret["F2"]=OIS::KC_F2;
		ret["F3"]=OIS::KC_F3;
		ret["F10"]=OIS::KC_F10;
		ret["F11"]=OIS::KC_F11;
		ret["F12"]=OIS::KC_F12;

		ret["NUMPAD_ENTER"]=OIS::KC_NUMPADENTER;
	}

	return ret;
}

/**Inizialize on first use the vector of known key names for mouse buttons.
This key names must be used in the configuration files.*/
const std::map<std::string, OIS::MouseButtonID>& knownMouseButtonsNames(){
	static std::map<std::string, OIS::MouseButtonID> ret; //return value;
	static bool ready=false;
	
	if(ready==false){
		ret["LEFT_MOUSE_BUTTON"]=OIS::MB_Left;
	}

	return ret;
}


/**This constructor builds the map of with keys.
The map of keys is extracted by recognizing the keynames in the properties.*/
InputMapper::InputMapper() :
	leftMouseButtonDown(false)
{
	//Scan all the keys.
	std::map<std::string, OIS::KeyCode>::const_iterator k;
	for (k= knownKeyNames().begin(); k != knownKeyNames().end(); ++k){
			std::string configKeyName=k->first;
			OIS::KeyCode keyCode=k->second;

			//Lookup any matching action.
			std::string action = CONFIGURATION(configKeyName);
			if (!action.empty())
				this->registerActionKey(action, keyCode);
			else
			//Key with no action, logging the possible problem.
				LOG(WARNING) << "Found a key with no action!";
	}

	//Scan the mouse buttons.
	std::map<std::string, OIS::MouseButtonID>::const_iterator mb;
	for (mb=knownMouseButtonsNames().begin(); mb!=knownMouseButtonsNames().end(); ++mb){
		std::string action = CONFIGURATION(mb->first);
			if (!action.empty())
				this->registerActionKey(action, mb->second);
			else
			//Key with no action, logging the possible problem.
				LOG(WARNING) << "Found a mouse button with no action!";
	}
}

/************PLUGINS REGISTRATION*******************/

void InputMapper::registerToGetInput(Ogre::RenderWindow* listenToMe, OgreFacade* of){
	//get an handle to the window, and convert it to ois parameter format, so that it can go in the constructor
    ParamList pl;
    size_t windowHandle = 0;
    std::ostringstream windowHandleAsAString;
    listenToMe->getCustomAttribute("WINDOW", &windowHandle);
    windowHandleAsAString << windowHandle;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHandleAsAString.str()));

	this->oisInputManager = OIS::InputManager::createInputSystem( pl );

	this->oisKeyboard = static_cast<OIS::Keyboard*>(this->oisInputManager->createInputObject(OIS::OISKeyboard, true ));
	this->oisMouse = static_cast<OIS::Mouse*>(this->oisInputManager->createInputObject( OIS::OISMouse, true ));

    this->oisMouse->setEventCallback(this);
    this->oisKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(listenToMe);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(listenToMe, this);

	//Hack the initial mouse position (http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Initial+Mouse+Position+With+OIS+and+CEGUI)
	OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(this->oisMouse->getMouseState());
	mutableMouseState.X.abs = 400;
	mutableMouseState.Y.abs = 300;

	this->ogreFacade=of;

	LOG(INFO) << "input system registered";
}


void InputMapper::unregisterFromInput(){
	this->oisInputManager->destroyInputObject( this->oisKeyboard );
	this->oisInputManager->destroyInputObject( this->oisMouse );
	OIS::InputManager::destroyInputSystem(this->oisInputManager);
	LOG(INFO) << "Input objects destroyed";
}

/************INPUT CALLBACKS*******************/


/**Not really sure of what this does... It is needed to read the input.
It also iterates on the keys that are kept down.*/
bool InputMapper::frameRenderingQueued(const Ogre::FrameEvent& evt ){
	this->oisMouse->capture();
	this->oisKeyboard->capture();
	
	return true;
}


/**This function reacts to the pressure of keys.*/
bool InputMapper::keyPressed(const OIS::KeyEvent& evt){
    //detect the right fucntion, calls it
	LOG(INFO) << "Input received, key pressed " << evt.key;

	//calling the action executor to answer to this key
	callExecutor(evt.key, false);

	return true;
}

/**Key release callback.*/
bool InputMapper::keyReleased( const OIS::KeyEvent &arg )
{
	//reacting to the keyUp
	callExecutor(arg.key, true);

	return true;
}


/******Window managment (resize, close...)****/
/**Adjust mouse clipping area (code from tutorial application)*/
void InputMapper::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = this->oisMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

/**This always launch a shutdown command.
It is assumed that a plugin capable of executing it is available, and the "close all" action is bind
to the ESC key. It converts the "window closed" to a "esc key" event.*/
void InputMapper::windowClosed(Ogre::RenderWindow* rw){
	LOG(INFO) << "Window closed";
	/*Just call the executor for whatever is linked to the ESC key.
	Should not be hardcoded. But how many games let you configure the quit command?? This can be left.*/
	callExecutor(KC_ESCAPE, false);  

}


void InputMapper::callExecutor(OIS::KeyCode const& key, bool stop) const{

	//find the action linked to the key
	std::map<OIS::KeyCode, ActionName>::const_iterator actionIt = this->keyToAction.find(key);

	//Check if there exist an action to react
	if(actionIt== this->keyToAction.end()){
		//notice that this may not be an error: not all keys are used (would be useful to log the key).
		LOG(WARNING) << "Action not found for a key";
		return; //bail out
	}

	//Call the "standard processing" for an action.
	ActionName actionToDo = actionIt->second;
	callExecutor(actionToDo, stop);
}

void InputMapper::callExecutor(OIS::MouseButtonID const& key, bool stop) const {
	//Find the action linked to the mouse button.
	std::map<OIS::MouseButtonID, ActionName>::const_iterator actionIt = this->mouseButtonToAction.find(key);

	//Check if there exist an action to react
	if(actionIt == this->mouseButtonToAction.end()){
		LOG(WARNING) << "Action not found for a key mouse";  //notice that this may not be an error: not all keys are used (would be useful to log the key).
		return; //bail out
	}
	//Call the "standard processing" for an action.
	ActionName actionToDo = actionIt->second;
	callExecutor(actionToDo, stop);
}

void  InputMapper::callExecutor(const ActionName& actionToDo, bool stop) const{
	//lookup the executor and call it
	std::map<ActionName, ActionProviderPlugin*>::const_iterator executorIt = this->actionToExecutor.find(actionToDo);

	//this should never happen (every registered action come with an executor).
	assert(executorIt!=this->actionToExecutor.end());

	//If the flow gets there the executor is valid and can be used
	ActionProviderPlugin* executor= executorIt->second;
	assert(executor != 0);

	//And finally, do the thing!
	executor->doAction(actionToDo, stop);
}

void InputMapper::registerActionKey(ActionName const& action, OIS::KeyCode key){
	this->keyToAction[key]=action;
}

void InputMapper::registerActionKey(ActionName const& action, OIS::MouseButtonID button){
	this->mouseButtonToAction[button]=action;
}

void InputMapper::registerActionsProvider(ActionProviderPlugin* plugin){
	//Keyboard commands.
	std::vector<ActionName> actions = plugin->declareActions();
	for(std::vector<ActionName>::const_iterator it=actions.begin();
		it != actions.end();
		++it){
			this->actionToExecutor[*it]=plugin;
	}
	//Mouse movement.
	if (plugin->isMouseListener()){
		this->mouseListeners.push_back(plugin);
	}
}

bool InputMapper::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(  arg.state.buttonDown(OIS::MB_Left)  && leftMouseButtonDown == false) //Left button pushed.
	{
		leftMouseButtonDown = true; //Record state for release call.
		callExecutor(OIS::MB_Left, false);
	}
	//Other buttons not supported by now.
    return true;
}

bool InputMapper::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!arg.state.buttonDown(OIS::MB_Left) && leftMouseButtonDown == true) //Left button raised.
	{
		leftMouseButtonDown = false; //Reset state.
		callExecutor(OIS::MB_Left, true);
	}
	//Other buttons not supported by now.
    return true;
}

bool InputMapper::mouseMoved( const OIS::MouseEvent &arg )
{
	static ActionProviderPlugin::MousePosition mousePosition; //Master copy of the mouse position (should automatically init to (0,0).

	mousePosition.xPixels = arg.state.X.abs; //Rel is the delta. Position is in pixels.
	mousePosition.yPixels = arg.state.Y.abs;

	mousePosition.xNorm = (mousePosition.xPixels / (double) 800) * 2 - 1; //TODO: remove hardcoded screen size.
	mousePosition.yNorm = (mousePosition.yPixels / (double) 600) * 2 - 1;

	static const int kMouseStepSize = 120; //120 seems to be the step value... TODO: remove hardcoded.
	if ((arg.state.Z.rel % kMouseStepSize) != 0 ){
		/*I assumed that in all systems the mouse wheel moves with relative increments of 120 "units"
		  because it does so on my system. Could not find docs about this.*/
		static const std::string errorMessage = "Programming error! Unexpected mouse step.";
		LOG(ERROR) << errorMessage;
		throw std::range_error(errorMessage);
	}

	mousePosition.wheelSteps = arg.state.Z.rel / kMouseStepSize; 	
	
	//Call all the listeners
	std::vector<ActionProviderPlugin*>::iterator listener;
	for (listener = this->mouseListeners.begin(); listener != this->mouseListeners.end(); ++listener)
	{
		assert((**listener).isMouseListener()); //Somehow a non mouse listener ended up with the listener. Programming error!
		(**listener).mouseMove(mousePosition);
	}

    return true;
}






