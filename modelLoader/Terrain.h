#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include "Arrays.h"
#include "Vector3D.h"
#include "Size2D.h"
#include "ImageLoader.h"

class Terrain
{
public:

	~Terrain();
	static Terrain* loadTerrain(std::string _filename, int _size);
	void computeNormals();
	void draw();

	int getWidth();
	int getLength();
	float getHeight(int _w, int _l);
	void setHeight(int _w, int _l, float _h);
	Vector3D getNormal(int _w, int _l);

private:
	Terrain(int _wid, int _len);
	int m_width;
	int m_length;
    float** m_heights;
    Vector3D** m_normals;
	bool m_computedNormals;
};
#endif //TERRAIN_H
