This is the part of the game that converts user input into game events. Here we deal with people bashing into the keyboard.

## Goal
The input subsystem must:
* allow the configuration of key bynding (user can customize)
* be flexible
	* we do not know what the user controls will be (what the player will do in the game, and as such which keys he will press)
	* we do not know what will be on the "receiving side" of the input (what game objects are controlled by the user)

## Architecture
The bottom layer is the OIS system provided with OGRE. We get the "raw" input (key pressure, mouse movement...) from there. The input mapper is our listener, and has to "find the meaning" of what it receives. That is, map the input to something meaningful for the game.

A bit of "glossary":
*Action: something meaningful in the game world that the player can do ("fire the gun").
*Input mapper: is the thingy that transfors raw input ("spacebar pressed") to the correnspondent action("fire the gun").
*Action executor or provider: a software object that contains the code to implement an action ("gun.fire()").

We use a plugin architecture. Actions are provided by game objects (using components). The game objects register its actions ("I can fire a gun...") and provides the plugin to implement them ("...call this software object to fire it"). The input mapper knows the related key ("fire a gun: press spacebar"), looks up at runtime the executor and calls it.

The binding between action and key can be changed at runtime. The executor of an action is "fixed" and can't be changed after the registration (this may change if needed - a way of un-registering plugin and dealing with no-more-available actions would be required).
If the action starts when the key is pressed and stops when it is released ("fire the machine-gun") then two separate actions have to be recorded ("press trigger"/"release trigger"); there may be a single executor for both, however. This really depends on the plugin writer.

The InputMapper is NOT responsible for the life of the plugins. It will not delete them.

## In the code...
The interface of actions plugin is defined in the ActionProviderPlugin class. Extends this one to create a plugin.
The type for action names is ActionName (typedef to string).
The input mapper method registerActionsProvider cares of the registration. It calls declareActions in the plugin to get the actions that it provides.
The action-key binding is managed by the input mapper (keyToAction map). In the configuration file there are entries that link action names to key names. Those are loaded like any other property. The input mapper has a table that maps the key names to the OIS key codes (the key names are used only in the config file). On creation it scans the program options from the configuration store and for each key name that it finds it looks up the matching key code. Finally it stores the key code/action pair.
This system can be overriden in the code using the registerActionKey(key code, action name) method.

## What is left to do
In the future (and on a "if-needed" basis):
*mouse support
*key combo support
*ctrl, alt, shift...
*extension for AI/network support? We could send requests to perform actions from the AI or the network and use the executors for non-player objects (is this the best way? Is this too fine-grained?)
*plugin de-registration