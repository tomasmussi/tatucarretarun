// Coded by Mariano M. Chouza (http://www.chouza.com.ar) replacing a version 
// present in "Sol's 2d gl basecode 2.0" (http://www.iki.fi/sol/)

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <fstream>
#include <SDL.h>

class BinFile
{
	std::fstream s_;
public:
	BinFile(const char *filename);
    virtual ~BinFile();
    char readbyte();
    int16_t  readword();
    int32_t  readdword();
    void readbytes(char* buffer, size_t num);
	size_t tell();
    void seek(size_t pos);
};

#endif
