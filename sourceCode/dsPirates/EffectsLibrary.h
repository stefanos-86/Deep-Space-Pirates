#pragma once

#include <string>
#include <map>

#include "IDGenerator.h"
#include "dsMath.h"

#include "TimedElement.h"

namespace Ogre{
	class SceneManager;
}

/**This class is the one that deals with visual effects that are to be fired "on event".
By now this means particle effects. No idea if there will be other kinds, no idea if this is the right way of implementing the thing.
*/
class EffectsLibrary : TimedElement
{
public:
	/**Sets the known effects (hardcoded here) in the object.*/
	EffectsLibrary(Ogre::SceneManager* sceneManager);

	/**Plays the desired effect.
	Throws std::runtime_error if the efect is unknown.
	We do not provide constants or enums with the valid effect so that the clients can just invoke the method in the OgreFacade
	and stay "unaware" of the EffectsLibrary. Intentionally giving away the compile time check (enum) to keep some simplicity.
	Implementation limits:
	-1 emitter per system
	-deterministic time to live and duration (no "duration_min" etc. etc.).
	-no unlimited duration effect (don't want to deal with special cleanup cases).
	
	"Exausted" effects are automatically removed when finished (on timeout).
	*/
	void startEffect(const std::string& name, const Vector3d& position);
	
	/**Removes the "spent" effects when it is time do do so.*/
	virtual void timedCallback(numeric_id_t id, float actualDuration);

	/**Allows the ogre facade to pass a reference to the */

private:
	///Links the name of the offect to the Ogre particle template that implements it.
	std::map<std::string, std::string> name2template;

	///Generate the particle system names.
	IDGenerator particleNameMaker;

	///The particle systems are destroyed when expired. This links the destruction timers and the effects to destroy.
	std::map<numeric_id_t, std::string> timerId2EffectName;

	///The scene manager, to attach the effects. NOT owned by this object.
	Ogre::SceneManager* sceneManager; 
};

