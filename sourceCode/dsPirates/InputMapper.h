/**@file */

#pragma once
#include "stdafx.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <map>

#include "ActionName.h"

//there are various a circular dependency there. This forward references replaces some #include
class OgreFacade;
class ActionProviderPlugin;


/**This class listens to the input and transforms basic "raw" input (keys pressed, mouse movement...)
into "high level" game event (fire, turn...).
This class resolves only the "raw" to "high level" conversion. The actual execution
of the commands is done by the game logic, by registering input plugins to the input mapper that can execute actions.
Also the bindyng between action names and actual keys is resolved here (hardcoded, by now).*/
class InputMapper : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputMapper();

	/**************OIS**************/

	/**Registration code obtained by "trimming"/commenting the base application from tutorial.
	@param listenToMe the window from where input is collected
	@param  of when the ogrefacade order to register, it also gives a pointer to itself, for future use*/
	void registerToGetInput(Ogre::RenderWindow*, OgreFacade*);
	/**Detach the OIS stuff*/
	void unregisterFromInput();
	
	/************** Input plugins **************/
	
	/**This method allows the binding of actions to key.
	If a key already has an assigned action the action will be replaced (allow key remapping at runtime).
	@param action the action triggered by the key
	@param key the key to use to activate the action*/
	void registerActionKey(ActionName const& action, OIS::KeyCode key);

	/**This method allows the binding of actions to a mouse button.
	If a key already has an assigned action the action will be replaced (allow key remapping at runtime).
	@param action the action triggered by the key
	@param mouseButton the mouseButton to use to activate the action*/
	void registerActionKey(ActionName const& action, OIS::MouseButtonID button);

	/**Use this method to hook an action provider to the input mapper. Also set up the mouse listener.
	There is no un-registration method. Plugins are supposed to be "constant".
	There is no run time check either.
	@param plugin: the action provider to register*/
	void registerActionsProvider(ActionProviderPlugin* plugin);

private:
	//sub-systems that may be passed to the commands
	OgreFacade* ogreFacade;

	//OIS stuff needed to keep things working
	OIS::InputManager* oisInputManager;
	OIS::Mouse*    oisMouse;;
    OIS::Keyboard* oisKeyboard;

	/************** callbacks to override **************/
	  // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt );

	//input plugins and actions are stored here
	std::map<OIS::KeyCode, ActionName> keyToAction;
	std::map<OIS::MouseButtonID, ActionName> mouseButtonToAction;  
	std::map<ActionName, ActionProviderPlugin*> actionToExecutor;
	std::vector<ActionProviderPlugin*> mouseListeners;

	/**This function encapsulates the key->executor lookup and executor call*/
	void callExecutor(OIS::KeyCode const& key, bool stop) const;

	/**This function encapsulates the key->executor lookup and executor call for mouse commands.*/
	void callExecutor(OIS::MouseButtonID const& key, bool stop) const;

	/**Common "core" of all the callExecutor variants.*/
	void callExecutor(const ActionName& actionToDo, bool stop) const;


	/**The OIS library only has methods to know if a mouse button is down.
	When a mouse button is released, no buttons are down and there is no other indication about the state.
	This variable allows to understand if the button was down before the mouseReleased callback.*/
	bool leftMouseButtonDown;
	
};


