#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <iostream>


#include "Vector3D.h"
#include "fileHandler.h"
#include "Mesh.h"
#include "MD2Normals.h"
#include "MD2Mesh.h"


class MeshLoader
{
	bool isValidMD2File(char* _type, int _version);
public:
	Mesh* load3DS(std::string _path);
	//MD2::MD2Mesh* loadMD2(std::string _path);
	//void loadMD2_(std::string _path);
};

#endif
