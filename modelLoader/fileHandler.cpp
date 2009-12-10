
#include "fileHandler.h"

fileHandler::fileHandler()
{
	m_objFile = new std::ifstream();
	m_fileSize = 0;
	m_fileData = NULL;
	m_isOpen = FAILED;
}

void fileHandler::openFile(std::string _texPath)
{
	//open the binary file for input/output operations
	m_objFile->open(_texPath.c_str(), std::ios::binary|std::ios::ate);

	// error opening the File
	if (!m_objFile->is_open())
	{
		m_isOpen = FAILED;
		std::cout << "ERROR: cannot open the file " << _texPath.c_str() << "! Press any key to exit";
		return;
	}

	// get the file size
	m_fileSize = m_objFile->tellg();

	m_objFile->seekg(0, std::ios::beg);

	// read all the file and get its data
	m_fileData = new unsigned char[m_fileSize];
	m_objFile->read(reinterpret_cast<char*>(m_fileData),m_fileSize);

	// close the file
	m_objFile->close();
	m_isOpen = SUCCESS;
}

bool fileHandler::isOpen()
{
	return m_isOpen;
}

unsigned char* fileHandler::getFileData()
{
	return m_fileData;
}

int fileHandler::getFileSize()
{
	return m_fileSize;
}

fileHandler::~fileHandler()
{
	delete [] m_fileData;
	delete m_objFile;
}
