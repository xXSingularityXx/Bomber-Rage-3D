#ifndef OBJECTS_PROP_H
#define OBJECTS_PROP_H

#include "Vector3D.h"
#include <iostream>
#include <vector>

// vertex struct
	typedef struct{
		float x, y, z;
	} vertexCoord;

	typedef struct {
		std::string name;
		std::vector<vertexCoord> vertices;
		std::vector<Vector3D> normal;
	} frame;

	// a triangle struct
	typedef struct{
		int a,b,c;
		int x,y,z;	//index of the textures coord in each vertex
	} polygonCoord;

	// texture map coord
	typedef struct{
		float u, v;
	} textureCoord;

class Mesh
{
	public:

	Mesh();
	void addVertexCoord(vertexCoord _coord);
	void addNormalVector(Vector3D _vector);
	void addPolygonCoord(polygonCoord _coord);
	void addTextureCoord(textureCoord _coord);
	void addFrame(frame _frame);

	void Mesh::setFileName(std::string _fileName);
	void Mesh::setMeshName(std::string _fileName);
	void Mesh::setStartFrame(int _startFrame);
	void Mesh::setEndFrame(int _endFrame);
	int Mesh::getStartFrame();
	int Mesh::getEndFrame();

	void Mesh::setAnimationTime(float _time);
	float Mesh::getAnimationTime();

	int getVertexQty();
	int getPolygonsQty();
	int getTextureQty();
	int getFramesQty();

	vertexCoord getVertexFromIndex(int _index);
	polygonCoord getPolygonFromIndex(int _index);
	textureCoord getTextureFromIndex(int _index);
	frame getFrameFromIndex(int _index);

	private:
	std::string m_FileName;
	std::string m_MeshName;

	int m_startFrame;
	int m_endFrame;
	float m_time;

	int m_vertices_qty;
	int m_polygons_qty;
	int m_texture_qty;
	int m_frames_qty;
	int m_glCommands_qty;

	std::vector<vertexCoord> m_vertices;
	std::vector<Vector3D> m_normals;
	std::vector<polygonCoord> m_polygons;
	std::vector<textureCoord> m_texture;
	std::vector<frame> m_frames;

};

#endif
