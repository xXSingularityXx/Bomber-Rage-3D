
#include "ObjectManager.h"
#include "ImageLoader.h"
ObjectManager::ObjectManager()
{

}
/*
	Load a mesh with texture coordinates.
	keyframe animations not supported yet.
*/
Node* ObjectManager::createNode(std::string _path)
{
	MeshLoader* newMesh = new MeshLoader();
	std::string fileFormat;

	for (int i=_path.length()-1; i>0; i--)
	{
		if (_path[i] == '.') break;
		fileFormat.insert(fileFormat.begin(),_path[i]);
	}

	// return false if strings are equal!? wtf!...
	if (!fileFormat.compare("3DS"))
	{
		Mesh* objMesh = newMesh->load3DS(_path);
		StaticNode* node = new StaticNode(objMesh);

		return node;
	}
	else if (!fileFormat.compare("MD2"))
	{
		MD2::MD2Mesh* mesh = new MD2::MD2Mesh();
		mesh->loadMD2(_path);
		AnimatedNode* node = new AnimatedNode(mesh);

		return node;
	}
	else
	{
		std::cout << "Can't open file with format ." << fileFormat.c_str() << "\nPress any key to exit.";
		std::cin.get();

		return NULL;
	}

	return NULL;
}

Terrain* ObjectManager::createTerrain(std::string _path, int _size)
{
	Terrain* t = Terrain::loadTerrain(_path, _size);
	return t;

}

Texture* ObjectManager::createTexture(std::string _path)
{
	ImageLoader* bmp = new ImageLoader();
	unsigned int idTexture = bmp->loadBMP(_path);

	if (idTexture != NULL){
		Texture* texture = new Texture();
		texture->setTextureId(idTexture);
		return texture;
	}
	else
		return NULL;
}
