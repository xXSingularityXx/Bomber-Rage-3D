#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <fstream>
#include <iomanip>

class fileHandler
{
	public:

	const static bool SUCCESS = true;
	const static bool FAILED  = false;

	fileHandler();
	~fileHandler();
	void openFile(std::string _texPath);
	unsigned char* getFileData();
	int getFileSize();
	bool isOpen();

	private:
	std::ifstream* m_objFile;
	std::ifstream::pos_type m_fileSize;
	unsigned char* m_fileData;
	bool m_isOpen;
};

#endif
