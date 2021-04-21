#include "stdafx.h"

#include "dsMath.h"

/**@file
This file provide a "placeholder" for the little "helper functions" that we added to the geometry entities.
*/

void cleanZero(Vector3d*v){
	if(abs(v->x) < epsilon) v->x=0.0f;
	if(abs(v->y) < epsilon) v->y=0.0f;
	if(abs(v->z) < epsilon) v->z=0.0f;
}

void cleanZero(dsQuaternion *v){
	if(abs(v->x) < epsilon) v->x=0.0f;
	if(abs(v->y) < epsilon) v->y=0.0f;
	if(abs(v->z) < epsilon) v->z=0.0f;
	if(abs(v->w) < epsilon) v->w=0.0f;
}