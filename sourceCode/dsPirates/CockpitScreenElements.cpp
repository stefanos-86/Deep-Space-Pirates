#include "StdAfx.h"
#include "CockpitScreenElements.h"

#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>

#include <string>

using namespace Ogre;

//Names of onscreen elements;
static const std::string kMainOverlayName = "CockpitMainOverlay";
static const std::string kMFDName = "MFD";
static const std::string kMasterOnName = "masterOn";
static const std::string kMasterOffName = "masterOff";
static const std::string kMasterOnAltName = "masterAlt";
static const std::string kMFDTextName = "B_MFDText";				//Order of names = z-order of elements.
static const std::string kMFDTextBackgroundName = "A_MFDText";		//Order of names = z-order of elements.
static const std::string kRadarName = "Radar";
static const std::string kGunSightName = "Crosshair";
static const std::string kSteeringReticle = "RotationReticle";
static const std::string kMousePointer = "MousePointer";

//TODO! Hardcoded positions!!! Has to be changed to support screen resizing!

//Font used for any text in the HUD.
static const std::string kFontName = "HUD_MFD_Font";

CockpitScreenElements::CockpitScreenElements() :
	overlayManager(OverlayManager::getSingleton()),
	MFD(0),
	radar(0)
{
		//Load the font. TODO: move in a "load phase" in the facade?
	FontManager::getSingleton().load(kFontName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	//Initialize the product elements.
	this->masterAlternative = 0;
	this->masterOff = 0;
	this->masterOn = 0;
	this->MFDText = 0;
	this->mainOverlay = overlayManager.create(kMainOverlayName);



	//Control the construction of the game HUD elements. By now the steps are fixed, but this may change (e.g. different instruments).
	this->buildMFDPanel();
	this->buildMasterAlarm();
	this->buildMFDText();
	this->buildRadar();
	this->buildCrosshair();
	this->buildSteeringReticle();
	this->buildTemporaryCreditScreen();
	this->buildMousePointer();

	//Show the global overlay that contains all the elements.
	this->mainOverlay->show();
}


/*Public interface*/

void CockpitScreenElements::addRadarDot(double horizontalAngle, double verticalAngle, bool front, const std::string& name){
	assert(this->radar != 0); //Programming error! buildRadar must be called before addRadarDot.
	assert(! name.empty());	//There must be a valid name.
	assert(horizontalAngle <= 1 && horizontalAngle >= -1); //The parameter is outside the allowed limit.
	assert(verticalAngle <= 1 && verticalAngle >= -1); //The parameter is outside the allowed limit.

	//TODO: consider a flywheight! Why re-create the radar dots every time instead of just hiding them?

	//Convert to screen coordinates. (TODO: not sure if this scaling should go there - but this class already has all the hardcoded stuff...).
	//Scale to screen width.
	//Translate into the radar centre on the screen keeping into account the dot width.
	//Invert vertical pos (positive angle = up = small top)
	static const double angle2screenRatio = 70/2; //Screen is roughly 65 pixels. "Angular" position goes from -1 to +1.
	int posVertical =  static_cast<int>(-verticalAngle * angle2screenRatio) + 48;  //TODO!!! Very bad! Hardcoded texture dimensions!
	int posHorizontal = static_cast<int>(horizontalAngle * angle2screenRatio) + 51;

	if(!front){
		//Move the contact on the rear screen with an additional horizontal translation.
		posHorizontal+=94; //TODO!!! Very bad! Hardcoded texture dimensions!
		posVertical+=1; //...and adjust for a texture problem (or is it due to the scaling of the texture?).
	}
	
	//DO NOT use the common builder
	//	OverlayContainer* dot = this->CommonBuildingSteps(name, posHorizontal, posVertical, 3, 3, "RadarDot");
	//It adds the dot to the main overlay and this results in a exception at shutdown.
	OverlayContainer* dot = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", name));
	dot->setMetricsMode(GMM_PIXELS);
	dot->setVerticalAlignment(GuiVerticalAlignment::GVA_TOP);
	dot->setHorizontalAlignment(GuiHorizontalAlignment::GHA_LEFT);
	dot->setPosition(posHorizontal, posVertical);
    dot->setDimensions(3,3);
	dot->setMaterialName("RadarDot");

	//Put the dot in the radar
	this->radar->addChild(dot);
}

void CockpitScreenElements::removeRadarDots(){
	OverlayContainer::ChildContainerIterator cursor = this->radar->getChildContainerIterator();
	while(cursor.hasMoreElements()){
		std::string currentName = cursor.current()->first;
		cursor.getNext();
		this->radar->removeChild(currentName);	//This barbaric removal sequence is anohter good reason to use a flyweight and just hide things.
		this->overlayManager.destroyOverlayElement(currentName);
	}
}

void CockpitScreenElements::setMasterAlarmOn(){
	this->masterOff->hide();
	this->masterOn->show();
	this->masterAlternative->hide();
}
void CockpitScreenElements::setMasterAlarmOff(){
	this->masterOff->show();
	this->masterOn->hide();
	this->masterAlternative->hide();
}
void CockpitScreenElements::setMasterAlarmAlternative(){
	this->masterOff->hide();
	this->masterOn->hide();
	this->masterAlternative->show();
}

void CockpitScreenElements::showCredits(){
	this->temporaryCredits->show();
}
void CockpitScreenElements::hideCredits(){
	this->temporaryCredits->hide();
}

void CockpitScreenElements::printOnMFD(const std::string& line1, const std::string& line2, const std::string& line3){
	std::stringstream fullText;
	fullText <<  line1.substr(0,15) << std::endl << line2.substr(0,15) << std::endl << line3.substr(0,15);
	this->MFDText->setCaption(fullText.str());
}

void CockpitScreenElements::positionDirectionPointer(int x, int y){
	//The mouse pointer has a center alignement. Must translate the coordinates.
	this->mousePointer->setPosition(x - 400 - 16, y - 300 - 16 ); //TODO: assume a 800*600 window and a 32*32 cursor...
}

/*Construction functions*/
//TODO! Hardcoded positions!!! Has to be changed to support screen resizing!

OverlayContainer* CockpitScreenElements::CommonBuildingSteps(const std::string& elementName, double left, double top, double width, double height, const std::string& materialName){
	OverlayContainer* newPanel = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", elementName)); 
    newPanel->setMetricsMode(GMM_PIXELS);
	newPanel->setVerticalAlignment(GuiVerticalAlignment::GVA_BOTTOM);
    newPanel->setPosition(left, top);
    newPanel->setDimensions(width, height);
	newPanel->setMaterialName(materialName);
	this->mainOverlay->add2D(newPanel);
	return newPanel;
}

void CockpitScreenElements::buildMFDPanel(){
	this->MFD = this->CommonBuildingSteps(kMFDName, 0, -100, 500, 100, "MFD");
}

void CockpitScreenElements::buildMasterAlarm(){
	static const double alarmLeft = 310;
	static const double alarmTop = -87;
	static const double alarmWidth = 80;
	static const double alarmHeight = 75;
	this->masterOff = this->CommonBuildingSteps(kMasterOffName, alarmLeft, alarmTop, alarmWidth, alarmHeight, "Master_Alarm_Off");
	this->masterOn = this->CommonBuildingSteps(kMasterOnName, alarmLeft, alarmTop, alarmWidth, alarmHeight, "Master_Alarm_On");
	this->masterAlternative = this->CommonBuildingSteps(kMasterOnAltName, alarmLeft, alarmTop, alarmWidth, alarmHeight, "Master_Alarm_Alternative");
}

void CockpitScreenElements::buildMFDText(){
	//The name must sort AFTER the name of the background text. The order of the names defines the Z order!
	TextAreaOverlayElement* textArea = static_cast<TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", kMFDTextName));
	Ogre::TextAreaOverlayElement* textAreaBackground = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", kMFDTextBackgroundName));
	configureTextArea(textArea, kFontName);
	configureTextArea(textAreaBackground, kFontName);

	//Real text area: start empty.
	textArea->setColour(Ogre::ColourValue(0,1,0));
    textArea->setCaption("");
	this->MFDText = textArea;

	//Use a identical text area to paint the "off segments" in the display, darker than the text, in the background.
	textAreaBackground->setColour(Ogre::ColourValue(0,0.2,0));
    textAreaBackground->setCaption("888888888888888\n888888888888888\n888888888888888");  // 3 lines, 15 chars each.
}

void CockpitScreenElements::configureTextArea(TextAreaOverlayElement* area, const std::string& font)
{
	area->setMetricsMode(GMM_PIXELS);
    area->setPosition(45, 20);
	area->setDimensions(120, 300); 
    area->setCharHeight(20);
    area->setFontName(font); 

	assert(this->MFD != 0); //Programming error! buildMFDText called before buildMFDPanel!
	this->MFD->addChild(area);
}

void CockpitScreenElements::buildRadar(){
	this->radar = this->CommonBuildingSteps(kRadarName, 500, -100, 200, 100, "Radar");
}

void CockpitScreenElements::buildCrosshair(){
	OverlayContainer* sightPanel = this->CommonBuildingSteps(kGunSightName, -32, -32, 64, 64, "Sight");
	sightPanel->setVerticalAlignment(GuiVerticalAlignment::GVA_CENTER);
	sightPanel->setHorizontalAlignment(GuiHorizontalAlignment::GHA_CENTER);
}

void CockpitScreenElements::buildSteeringReticle(){
	OverlayContainer* rotationHud = this->CommonBuildingSteps(kSteeringReticle, -128, -128, 256, 256, "RotationReticle");
	rotationHud->setVerticalAlignment(GuiVerticalAlignment::GVA_CENTER);
	rotationHud->setHorizontalAlignment(GuiHorizontalAlignment::GHA_CENTER);
}

void CockpitScreenElements::buildMousePointer(){
	OverlayContainer* mousePointer = this->CommonBuildingSteps(kMousePointer, -16, -16, 32, 32, "MousePointer");
	mousePointer->setVerticalAlignment(GuiVerticalAlignment::GVA_CENTER);
	mousePointer->setHorizontalAlignment(GuiHorizontalAlignment::GHA_CENTER);
	mousePointer->setPosition(-16,-16);
	this->mousePointer = mousePointer;
}

//TEMPORARY!!!
void CockpitScreenElements::buildTemporaryCreditScreen(){
	OverlayContainer* temporaryCreditScreen = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "temporaryCreditScreen")); 
    temporaryCreditScreen->setMetricsMode(GMM_PIXELS);
	temporaryCreditScreen->setVerticalAlignment(GuiVerticalAlignment::GVA_CENTER);
	temporaryCreditScreen->setHorizontalAlignment(GuiHorizontalAlignment::GHA_CENTER);
    temporaryCreditScreen->setPosition(-250, -250);
    temporaryCreditScreen->setDimensions(512,512);
	temporaryCreditScreen->setMaterialName("temporaryCreditScreen");
	this->temporaryCredits = temporaryCreditScreen;
	this->mainOverlay->add2D(temporaryCreditScreen);
}
