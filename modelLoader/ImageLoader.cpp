
#include "ImageLoader.h"


/*
	get the data from the .BMP file
*/

unsigned int ImageLoader::loadBMP(std::string _texPath)
{
	unsigned char* fileData;

	//load the BMP file
	fileHandler* objFile = new fileHandler();
	objFile->openFile(_texPath);

	if (objFile->isOpen() == fileHandler::FAILED)
		return NULL;

	fileData = objFile->getFileData();

    // structs from windows.h to use with .bmp
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

	// copy the data to its respectves headers
	memcpy(&fileheader, fileData, sizeof(fileheader));
	memcpy(&infoheader, fileData + sizeof(fileheader), sizeof(infoheader));

	// retrieve the size of the image in pixels
	m_texSize.height = (float)infoheader.biHeight;
	m_texSize.width = (float)infoheader.biWidth;
	m_texture = new unsigned char [(int) (m_texSize.height * m_texSize.width) * 4];

	int indexTexture=0;
	for (int i=0; i < m_texSize.height * m_texSize.width; i++)
	{
		memcpy(&rgb, fileData + sizeof(infoheader) + sizeof(fileheader), sizeof(rgb));

        m_texture[indexTexture+0] = rgb.rgbtRed; // Red component
        m_texture[indexTexture+1] = rgb.rgbtGreen; // Green component
        m_texture[indexTexture+2] = rgb.rgbtBlue; // Blue component
        m_texture[indexTexture+3] = 255; // Alpha value
        indexTexture += 4; // Go to the next position
		fileData += 3;
	}

	unsigned int tempIdTex=0;
	glGenTextures(1,&tempIdTex);
	glBindTexture(GL_TEXTURE_2D, tempIdTex);		//binds the name to the target

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_texSize.width, m_texSize.height, GL_RGBA, GL_UNSIGNED_BYTE, m_texture);

	return tempIdTex;
}

ImageLoader::~ImageLoader()
{
	delete [] m_texture;
}

unsigned char* ImageLoader::getPixels()
{
	return m_texture;
}

Size2D ImageLoader::getImageSize()
{
	return m_texSize;
}

