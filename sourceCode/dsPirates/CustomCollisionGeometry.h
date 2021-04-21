#pragma once

#include <vector>

//Gives havok types
#include <Physics/Collide/Shape/hkpShape.h>


#include "ICollisionGeometry.h"

class hkpExtendedMeshShape;

/**This class owns the geometry information for collisions for cases where Havok build-in shapes are not sufficient.
It can return the pointer to a array of vertices and to the face index as expected by havok.

Vertices are stored as consecutive triplets of hkReals (floats).
+----------+----------+---
| Vertex 0 | Vertex 1 | ... and so forth
| x | y | z| x | y | z|
+----------+----------+----
<--------->
   Stride (3*sizeof(float))

Faces are triangles and are indices are stored in a similar way, as hkInt32 (signed ints).
The OBJ convention is followed: indexes start from 1. To compensate, the 1st vertex (index 0) is left unused in the index)
+----------+----------+---
| Vertex 0 | Vertex 1 | ... and so forth
| x | y | z| x | y | z|
+----------+----------+----
<--------->
   Stride (3*sizeof(int))

Static members allow a easy loading of the geometry from a stripped-down version of the Wavefront OBJ format and
conversion to havok shapes. Havok shapes keeps pointer to the data inside this class: be aware of its lifetime!
Don't free/let go out of scope before the havok shape.
*/
class CustomCollisionGeometry : public ICollisionGeometry
{
public:
	
	~CustomCollisionGeometry(void);
	
	/**This method creates a collision shape based on the data in the file. Clients must delete the returned object.
	The file is a subset of a normal Wavefront OBJ.
	Only the lines starting with "v" and "f" are read. All the rest is ignored.
	This is the only data we need, and we want to keep things simple.*/
	static CustomCollisionGeometry* loadFromFile(const std::string& filename);

	/**Implements the collision geometry provider interface.*/
	virtual hkpShape* getCollisionGeometry();

private:
	/**This class can be constructed only by loading data from a file using the static method.*/
	CustomCollisionGeometry(void);

	///Holds the vertices.
	std::vector<hkReal> vertices;
	///Holds the faces.
	std::vector<hkInt32> faceIndices;
	///Holds the havok-ready data.
	hkpExtendedMeshShape* collisionGeometry;
};

