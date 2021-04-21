#include "StdAfx.h"
#include "gocVisualCamera.h"

gocVisualCamera::gocVisualCamera() {}
gocVisualCamera::~gocVisualCamera() {}

const gocVisualCamera::goc_id_type gocVisualCamera::componentID() const {
	return goc_id_type(GameObjectComponent::GOC_TYPE_VISUAL); 
}
