#include "stdafx.h"
#include "ActionProviderPlugin.h"

bool ActionProviderPlugin::isMouseListener() const{
	return false;
}
void ActionProviderPlugin::mouseMove(const MousePosition& currentMousePosition){
	assert(! isMouseListener()); //If this is a mouse listener, you should not use the "no-op" base implementation to react to the mouse.
	assert(isMouseListener()); //If this is NOT a mouse listener, the mouseMove should not be called - another programming error!
}