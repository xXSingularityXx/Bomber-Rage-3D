#include "Mesh.h"

Mesh::Mesh()
{
	m_vertices_qty=0;
	m_polygons_qty=0;
	m_texture_qty=0;
	m_frames_qty=0;

	m_time=0;
	m_startFrame=0;
	m_endFrame=0;
}

void Mesh::addVertexCoord(vertexCoord _coord)
{
	m_vertices.push_back(_coord);
	m_vertices_qty++;
}

void Mesh::addPolygonCoord(polygonCoord _coord)
{
	m_polygons.push_back(_coord);
	m_polygons_qty++;
}

void Mesh::addTextureCoord(textureCoord _coord)
{
	m_texture.push_back(_coord);
	m_texture_qty++;
}

void Mesh::addFrame(frame _frame)
{
	m_frames.push_back(_frame);
	m_frames_qty++;
	m_vertices_qty = _frame.vertices.size();
}


void Mesh::setFileName(std::string _fileName)
{
	m_FileName = _fileName;
}

void Mesh::setMeshName(std::string _fileName)
{
	m_MeshName = _fileName;
}

void Mesh::setStartFrame(int _startFrame)
{
	m_startFrame = _startFrame;
}

void Mesh::setEndFrame(int _endFrame)
{
	m_endFrame = _endFrame;
}


int Mesh::getVertexQty()
{
	return m_vertices_qty;
}

int Mesh::getFramesQty()
{
	return m_frames_qty;
}

int Mesh::getPolygonsQty()
{
	return m_polygons_qty;
}


vertexCoord Mesh::getVertexFromIndex(int _index)
{
	return m_vertices[_index];
}

polygonCoord Mesh::getPolygonFromIndex(int _index)
{
	return m_polygons[_index];
}

textureCoord Mesh::getTextureFromIndex(int _index)
{
	return m_texture[_index];
}

frame Mesh::getFrameFromIndex(int _index)
{
	return m_frames[_index];
}


void Mesh::setAnimationTime(float _time)
{
	m_time = _time;
}

float Mesh::getAnimationTime()
{
	return m_time;
}

int Mesh::getStartFrame()
{
	return m_startFrame;
}

int Mesh::getEndFrame()
{
	return m_endFrame;
}
