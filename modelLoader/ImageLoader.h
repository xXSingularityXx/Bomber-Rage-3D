#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <iostream>
#include <windows.h>
//#include <GL/glut.h>
#include <SDL/SDL_opengl.h>
#include "Size2D.h"
#include "fileHandler.h"

class ImageLoader
{
	private:
	Size2D m_texSize;
	unsigned char* m_texture;

	public:
	~ImageLoader();
	unsigned int loadBMP(std::string _texPath);
	unsigned int loadJPG(std::string _texPath);

	unsigned char* getPixels();
	Size2D getImageSize();

};

#endif
