#ifndef VIDEO_H
#define VIDEO_H

#include "config.h"

#include "loader.h"
#include <cmath>
#include <fstream>
#include "SDL.h"
#include "SDL_image.h"
#include <SDL_opengl.h>
#include <sstream>
#include <iostream>
#include <vector>

class Video
{
public:
	Video(const Config& config, const char* pathFondo);	
	GLuint fondo;
	const char* pathFondo;
	std::vector<GLuint> textures;
	void loadSurfaces();
	void loadSurface(const char* surf);
};

#endif
