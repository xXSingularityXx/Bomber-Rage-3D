
#include "MeshLoader.h"


Mesh* MeshLoader::load3DS(std::string _path)
{
	int i=0;
	int vertices_qty=0;
	int polygons_qty=0;
	int texture_qty = 0;
	std::string meshName="";

	Mesh* objectMesh = new Mesh();
	objectMesh->setFileName(_path);
		
	//load the 3ds file, open the file and get the data
	fileHandler* objFile = new fileHandler();
	objFile->openFile(_path);
	unsigned char* data = objFile->getFileData();
	int size = objFile->getFileSize();
	
	// initialize the chunk propriertes
	int chunk_id=0;
	int chunk_length=0;
	int countChunk=0;

	while (countChunk < size)
	{
		//get the chunk id
		memcpy(&chunk_id, data+countChunk, sizeof(short int));
		countChunk+=2;

		//get the length of the chunk
		memcpy(&chunk_length, data+countChunk, sizeof(int));
		countChunk += 4;
		
		// verify what type of chunk we got
		switch (chunk_id)
        {
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
			break;    

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;
			
			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
			case 0x4000: 
				while(data[countChunk] != '\0')
				{
					meshName.push_back(data[countChunk]);
					countChunk++;
				}
				objectMesh->setMeshName(meshName);
				countChunk++;
			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4100:
			break;
			
			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices) 
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
			case 0x4110: 
				vertices_qty=0;
				memcpy(&vertices_qty, data + countChunk, sizeof(short int));
				countChunk+=2;

				for (i=0; i<vertices_qty; i++)
				{
					vertexCoord vet;

					memcpy(&vet.x, data + countChunk, sizeof(float));
					countChunk+=4;

					memcpy(&vet.y, data + countChunk, sizeof(float));
					countChunk+=4;

					memcpy(&vet.z, data + countChunk, sizeof(float));
					countChunk+=4;

					//debug
					//std::cout << "x: " << vet.x << "\n";
					//std::cout << "y: " << vet.y << "\n";
					//std::cout << "z: " << vet.z << "\n\n";
					objectMesh->addVertexCoord(vet);
					//vertices.push_back(vet);
				}
			
				break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons) 
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
			case 0x4120:
				polygons_qty = 0;
				memcpy(&polygons_qty, data + countChunk, sizeof(short int));
				countChunk+=2;

				for (i=0; i<polygons_qty; i++)
				{
					polygonCoord pol;

					pol.a=0;
					memcpy(&pol.a, data+countChunk, sizeof(short int));
					countChunk+=2;

					pol.b = 0;
					memcpy(&pol.b, data+countChunk, sizeof(short int));
					countChunk+=2;

					pol.c = 0;
					memcpy(&pol.c, data+countChunk, sizeof(short int));
					countChunk+=4;

					//debug
					//std::cout << "a: " << pol.a << "\n";
					//std::cout << "b: " << pol.b << "\n";
					//std::cout << "c: " << pol.c << "\n\n";
					objectMesh->addPolygonCoord(pol);
				}
				
                break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points) 
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
			case 0x4140:

				texture_qty = 0;
				memcpy(&texture_qty, data+countChunk, sizeof(short int));
				countChunk+=2;

				for (i=0; i<texture_qty; i++)
				{
					textureCoord text;

					memcpy(&text.u, data+countChunk,sizeof(float));
					countChunk+=4;

					memcpy(&text.v, data+countChunk, sizeof(float));
					countChunk+=4;
										
					objectMesh->addTextureCoord(text);
				}

                break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
				countChunk += chunk_length-6;
        } 
	}
	delete objFile;

	return objectMesh;
}


bool MeshLoader::isValidMD2File(char* _type, int _version)
{
	bool isValidMD2File = true;
	bool isValidMD2Version = true;

	if (_type[0] != 'I' || _type[1] != 'D' ||
        _type[2] != 'P' || _type[3] != '2') 
	{
		isValidMD2File=false;
	}

	if (_version != 8 )
		isValidMD2Version=false;

	// if the type and version of the file are valid, the file is a valid MD2
	if (isValidMD2File && isValidMD2Version)
		return true;

	return false;
}
