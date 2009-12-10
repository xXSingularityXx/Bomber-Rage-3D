
#include "Terrain.h"

Terrain::Terrain(int _wid, int _len)
{
	m_width = _wid;
	m_length = _len;

	m_heights = new float*[m_length];
    for(int i = 0; i < m_length; i++) {
        m_heights[i] = new float[m_width];
    }

    m_normals = new Vector3D*[m_length];
    for(int i = 0; i < m_length; i++) {
        m_normals[i] = new Vector3D[m_width];
    }

	m_computedNormals=false;
}

Terrain::~Terrain()
{
	for(int i = 0; i < m_length; i++) {
        delete[] m_heights[i];
    }
    delete[] m_heights;

    for(int i = 0; i < m_length; i++) {
        delete[] m_normals[i];
    }
    delete[] m_normals;
}

int Terrain::getWidth()
{
	return m_width;
}

int Terrain::getLength()
{
	return m_length;
}

float Terrain::getHeight(int _w, int _l)
{
	return m_heights[_l][_w];
}

void Terrain::setHeight(int _w, int _l, float _h)
{
	m_heights[_l][_w] = _h;
	m_computedNormals=false;
}

Vector3D Terrain::getNormal(int _w, int _l)
{
	if (!m_computedNormals)
	{
         computeNormals();
    }
    return m_normals[_l][_w];
}

void Terrain::computeNormals()
{
	if (m_computedNormals) {
        return;
    }

    Vector3D** normals2 = new Vector3D*[m_length];
    for(int i = 0; i < m_length; i++) {
        normals2[i] = new Vector3D[m_width];
    }

    for(int z = 0; z < m_length; z++) {
        for(int x = 0; x < m_width; x++) {
            Vector3D sum(0.0f, 0.0f, 0.0f);

            Vector3D out;
            if (z > 0) {
                out = Vector3D(0.0f, m_heights[z - 1][x] - m_heights[z][x], -1.0f);
            }
            Vector3D in;
            if (z < m_length - 1) {
                in = Vector3D(0.0f, m_heights[z + 1][x] - m_heights[z][x], 1.0f);
            }
            Vector3D left;
            if (x > 0) {
                left = Vector3D(-1.0f, m_heights[z][x - 1] - m_heights[z][x], 0.0f);
            }
            Vector3D right;
            if (x < m_width - 1) {
                right = Vector3D(1.0f, m_heights[z][x + 1] - m_heights[z][x], 0.0f);
            }

            if (x > 0 && z > 0) {
				sum = sum + out.crossProduct(left).normalize();
            }
            if (x > 0 && z < m_length - 1) {
				sum = sum + left.crossProduct(in).normalize();
            }
            if (x < m_width - 1 && z < m_length - 1) {
                sum = sum + in.crossProduct(right).normalize();
            }
            if (x < m_width - 1 && z > 0) {
                sum = sum + right.crossProduct(out).normalize();
            }

            normals2[z][x] = sum;
        }
	}

	// SMOOTH THE NORMALS
    const float FALLOUT_RATIO = 0.5f;
    for(int z = 0; z < m_length; z++) {
        for(int x = 0; x < m_width; x++) {
            Vector3D sum = normals2[z][x];

            if (x > 0) {
                sum = sum + normals2[z][x - 1] * FALLOUT_RATIO;
            }
            if (x < m_width - 1) {
                sum = sum + normals2[z][x + 1] * FALLOUT_RATIO;
            }
            if (z > 0) {
                sum = sum + normals2[z - 1][x] * FALLOUT_RATIO;
            }
            if (z < 0) {
                sum = sum + normals2[z + 1][x] * FALLOUT_RATIO;
            }

			if (sum.getVectorLength() == 0) {
                sum = Vector3D(0.0f, 1.0f, 0.0f);
            }
            m_normals[z][x] = sum;
        }
    }

	for(int i = 0; i < m_length; i++)
	{
		delete[] normals2[i];
	}
	delete[] normals2;

	m_computedNormals =true;
}

Terrain* Terrain::loadTerrain(std::string _filename, int _size)
{
	ImageLoader* img = new ImageLoader();
	img->loadBMP(_filename.c_str());
	unsigned char* pixels = img->getPixels();
	Size2D imgSize = img->getImageSize();
	Terrain* t = new Terrain(imgSize.width, imgSize.height);

	for(int y = 0; y < imgSize.height; y++) {
        for(int x = 0; x < imgSize.width; x++) {
            unsigned char color =
                pixels[4 * (y * (int)imgSize.width + x)];
            float h = _size * ((color / 255.0f) - 0.5f);		//height goes from -0.5f to 0.5f
            t->setHeight(x, y, h);
        }
    }
	t->computeNormals();
	delete img;
	return t;
}

void Terrain::draw()
{
	glPushMatrix();
	glColor3f(0.3f, 0.9f, 0.0f);
	glTranslatef(-130, 0, -100);
    for(int z = 0; z < getLength() - 1; z++) {
        //Makes OpenGL draw a triangle at every three consecutive vertices
        glBegin(GL_TRIANGLE_STRIP);
        for(int x = 0; x < getWidth(); x++) {
            Vector3D normal = getNormal(x, z);
			//normal = normal.normalize();
			glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(x, getHeight(x, z), z);
            normal = getNormal(x, z + 1);
			//normal = normal.normalize();
			glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(x, getHeight(x, z + 1), z + 1);
        }
        glEnd();
    }

	glPopMatrix();
}

