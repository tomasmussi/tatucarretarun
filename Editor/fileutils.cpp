#include "fileutils.h"
#include <SDL.h>

BinFile::BinFile(const char *filename) :
s_(filename, std::ios::in | std::ios::binary)
{
}

BinFile::~BinFile()
{
}

char BinFile::readbyte()
{
	return s_.get();
}

int16_t BinFile::readword()
{
	uint16_t ret;
	s_.read(reinterpret_cast<char*>(&ret), sizeof(ret));
#if SDL_BYTEORDER != SDL_LIL_ENDIAN
    ret = SDL_Swap16(ret);
#endif
	return ret;
}

int32_t BinFile::readdword()
{
	uint32_t ret;
	s_.read(reinterpret_cast<char*>(&ret), sizeof(ret));
#if SDL_BYTEORDER != SDL_LIL_ENDIAN
    ret = SDL_Swap32(ret);
#endif
	return ret;
}

void BinFile::readbytes(char* buffer, size_t num)
{
	s_.read(buffer, num);
}

size_t BinFile::tell()
{
	return s_.tellg();
}

void BinFile::seek(size_t pos)
{
	s_.seekg(pos);
}
