#include "stdafx.h"
#pragma once



/**@file
We are using the Ogre vector library for vectors, and other mathematical ang geometrical entities. 
This "class" acts as a wrapper to decouple our code from Ogre, as we plan (in the far future) to use
a specialized math library (for practice, mostly).
*/

/**Gives a new name to the already available vector class.
Typedef would be better, but does not work. Are qualified name allowed?*/
#define Vector3d Ogre::Vector3 
/**Gives a new new name to the component of a vector.*/
typedef Ogre::Real vector_component;

/**Rename the actual quaternion implementation.
We need a dsPrefix - there is a clash with the havok quaternion.*/
#define  dsQuaternion Ogre::Quaternion
/**Rename the actual matrix implementation.*/
#define Matrix3x3  Ogre::Matrix3

/**Our conventional "so small that is practically zero" value. Anything smaller than this can be "rounded down" to zero.*/
const float epsilon=0.000001f;
/**Auxiliary function to cope with near-zero approximation errors. Set the "near zero" values in the vector to zero.*/
void cleanZero(Vector3d* v);
/**Auxiliary function to cope with near-zero approximation errors. Set the "near zero" values in the quaternion to zero.*/
void cleanZero(dsQuaternion *v);