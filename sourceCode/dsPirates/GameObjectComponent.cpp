#include "stdafx.h"

#include "GameObjectComponent.h"

/**@file
Even if the class is pure qbstract, there are some constants that require initialization.*/

/*Definition of class constants*/
const GameObjectComponent::goc_id_type GameObjectComponent::GOC_TYPE_VISUAL = "gocVisual";
const GameObjectComponent::goc_id_type GameObjectComponent::GOC_TYPE_PHYSICS = "gocPhysics";
const GameObjectComponent::goc_id_type GameObjectComponent::GOC_TYPE_LOGIC = "gocLogic";
const GameObjectComponent::goc_id_type GameObjectComponent::GOC_TYPE_AUDIO = "gocBumCrashSplatBangGnam"; //...this string is never, ever, directly used.
const GameObjectComponent::goc_id_type GameObjectComponent::GOC_TYPE_TEST_AND_DEBUG = "DEBUG_goc_type";

