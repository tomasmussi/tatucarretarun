#ifndef TEXTURE_STORE_H
#define TEXTURE_STORE_H

#include <map>
#include <string>
#include <SDL_opengl.h>

class TextureStore
{
	typedef std::map<std::string, GLuint> TTexStore;
	static TTexStore texStore_;
public:
	static GLuint addTexture(const std::string& filename);
	static GLuint getTexture(const std::string& filename);
};

#endif
