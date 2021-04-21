#include "StdAfx.h"
#include "CustomCollisionGeometry.h"
//The kind of havok shape returned by this class.
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/hkpShape.h>
//File operations and string manipulation.
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

CustomCollisionGeometry::CustomCollisionGeometry(void)
	:collisionGeometry(0)
{
}


CustomCollisionGeometry::~CustomCollisionGeometry(void)
{
	delete this->collisionGeometry;
}


CustomCollisionGeometry* CustomCollisionGeometry::loadFromFile(const std::string& filename){
	std::ifstream objFile(filename);
	std::string currentLine;

	std::vector<float> vertices;
	//Extra bit to align indices.
	vertices.push_back(0);vertices.push_back(0);vertices.push_back(0);
	std::vector<int> faces;
	while(std::getline(objFile, currentLine)){
		if (!currentLine.empty() && currentLine.at(0) == 'v'){
			//Separate the vertices.
			std::istringstream iss(currentLine);
			std::istream_iterator<std::string> tokens(iss);
			++tokens; //skip the 'v'
			vertices.push_back(atof((*tokens).c_str())); //X
			++tokens;
			vertices.push_back(atof((*tokens).c_str())); //Y
			++tokens;
			vertices.push_back(atof((*tokens).c_str())); //Z
		}
		if (!currentLine.empty() && currentLine.at(0) == 'f'){
			//Separate the vertices.
			std::istringstream iss(currentLine);
			std::istream_iterator<std::string> tokens(iss);
			++tokens; //skip the 'f'
			faces.push_back(atoi((*tokens).c_str())); //Vertex 1
			++tokens;
			faces.push_back(atoi((*tokens).c_str())); //Vertex 2
			++tokens;
			faces.push_back(atoi((*tokens).c_str())); //Vertex 3
		}
	} //line loop.

	//Create the return object (and pray not to leak it due to havok errors!)
	//TODO: this is not efficient. We should add a MOP (I believe).
	CustomCollisionGeometry* ret = new CustomCollisionGeometry();
	ret->vertices.resize(vertices.size());
	ret->faceIndices.resize(faces.size());
	std::copy(vertices.begin(), vertices.end(), ret->vertices.begin());
	std::copy(faces.begin(), faces.end(), ret->faceIndices.begin());

	//Now create a shape that points to the data in the return objects.
	static hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_vertexBase = reinterpret_cast<hkReal*>(&(ret->vertices[0]));
	part.m_vertexStriding = 3* sizeof(float);
	part.m_numVertices = (ret->vertices.size())/3;

	part.m_indexBase =(&(ret->faceIndices[0]));
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32; //bet on int width on all platforms... better go with havok types such as hkUint16
	part.m_indexStriding = 3 * sizeof(hkInt32);
	part.m_numTriangleShapes = (ret->faceIndices.size())/3;

	//Comment from the havok tutorial...
	// Using a mesh without a bounding volume can be very slow.
	// In this demo we don't care.
	hkpExtendedMeshShape* shape = new hkpExtendedMeshShape();
	shape->addTrianglesSubpart(part);
	ret->collisionGeometry = shape;

	return ret;

}

hkpShape* CustomCollisionGeometry::getCollisionGeometry(){
	return this->collisionGeometry;
}

