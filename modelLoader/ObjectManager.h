#ifndef SCENE_ENVIRONMENT_H
#define SCENE_ENVIRONMENT_H

#include <iostream>
#include <windows.h>
#include "Vector3D.h"
#include "Node.h"
#include "StaticNode.h"
#include "AnimatedNode.h"
#include "fileHandler.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Texture.h"
#include "MeshLoader.h"
#include "MD2Mesh.h"
#include "Terrain.h"

/*
	Load a Mesh from a file
*/

class ObjectManager
{
	public:

	ObjectManager();
	Node* createNode(std::string _path);
	Terrain* createTerrain(std::string _path, int _size);
	Texture* createTexture(std::string _path);

	private:

};
#endif
