#ifndef VIDEO_H
#define VIDEO_H

#include "config.h"

#include "loader.h"
#include <cmath>
#include <fstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <sstream>
#include <iostream>
#include <vector>

class Video
{
public:
	Video(const Config& config);
	virtual ~Video();
	GLuint texture;			// This is a handle to our texture object
	SDL_Surface *surface;	// This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;
	std::vector<GLuint> textures;
	void loadSurfaces();
	void loadSurface(char* surf);
};

#endif
