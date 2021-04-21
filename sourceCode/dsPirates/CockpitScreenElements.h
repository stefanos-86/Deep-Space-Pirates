#pragma once

#include <string>

namespace Ogre{
	class OverlayManager;
	class Overlay;
	class OverlayContainer;
	class TextAreaOverlayElement;
}

/** This class is a helper for the cockpit. It handles the "low level operations", like turn on a light, print some text.*/
class CockpitScreenElements{

public:
	CockpitScreenElements();

	///Add a radar dot on the radar screen. Can be called only after buildRadar.
	void addRadarDot(double horizontalAngle, double verticalAngle, bool front, const std::string& name); //TODO: replace bool with enum

	///A exception to the norma builder pattern. Removes all the radar dot elements.
	void removeRadarDots();

	void setMasterAlarmOn();
	void setMasterAlarmOff();
	void setMasterAlarmAlternative();

	///TEMPORARY
	void showCredits();
	void hideCredits();

	/**Raw input on the MFD.
	The Multi Function Display is a 3 line liquid crystal display. It can print everything so we can do
	a lot of stuff with a single widget...
	Each line has 15 chars. Extra chars are truncated.*/
	void printOnMFD(const std::string& line1, const std::string& line2, const std::string& line3);

	void positionDirectionPointer(int x, int y);

	///TODO: may want to have a "turnOnLight(name)" later on, if the number of elements becomes bigger.

private:
	/*Elements on the screen that requires action "later on" - fixed elements are stored only by Ogre, we don't keep references.*/
	Ogre::Overlay* mainOverlay;						//Contains all the other elements
	Ogre::OverlayContainer* masterOn;				//Master alarm when light is on.
	Ogre::OverlayContainer* masterOff;				//Master alarm when light is off.
	Ogre::OverlayContainer* masterAlternative;		//Master alarm when light is on - alternative text.
	Ogre::TextAreaOverlayElement* MFDText;			//Writable part of the MFD.
	Ogre::OverlayContainer* radar;					//Radar screen, can have dots added on it.
	Ogre::OverlayContainer* mousePointer;			//Something to know where the mouse is pointing.
	Ogre::OverlayContainer* temporaryCredits;		//TEMPORARY!!!
													/*THIS SHOULD NOT BE THERE! In order to speed up the alignement of the legal requirements we
													put a temporary credit screen here (just because this object is already dealing with overlays.
													This will be removed as soon as we add a proper GUI.*/

	/*Privately contained objects - even more private!*/
	///Actually creates stuff in Ogre.
	Ogre::OverlayManager& overlayManager;

	///This element may be acted upon by many methods, but it is not part of the output (the cockpit does not animate it).
	Ogre::OverlayContainer* MFD; //TODO: check, may be possible to ditch it...

	/*Construction functions to create all the elements. Expect those to be called in the constructor.*/

	///Creates the cockpit computer.
	void buildMFDPanel();

	///Creates the 3 "frames" of the master alarm (on/off/alternate).
	void buildMasterAlarm();

	///Creates two text areas over the MFD. One is "background", the other is in the ScreenElement to write the text.
	//Can be called only after buildMFDPanel.
	void buildMFDText();
	
	/**There are two text areas that must overlap and have the same font.*/
	void configureTextArea(Ogre::TextAreaOverlayElement* area, const std::string& font);

	///Creates the radar panel.
	void buildRadar();

	///Creates the gun crosshair, the aiming cross in the middle of the screen.
	void buildCrosshair();

	///Creates the reticle to help control rotation.
	//TODO: this is useless, as not calibrated to match the actual control system (plus, is very small). Can be removed, but is nice to see so I leave it.
	void buildSteeringReticle();

	///Creates the mouse pointer;
	void buildMousePointer();

	//TEMPORARY!!!
	///Creates the credit screen. This is not part of the cockpit, but this is the only pseudo-GUI we have at the moment. The credits are required to show the Havok logo (it's in the licese).
	void buildTemporaryCreditScreen();

	///Factor operations that are common for all elements.
	Ogre::OverlayContainer* CommonBuildingSteps(const std::string& elementName, double left, double top, double width, double height, const std::string& materialName);

};

