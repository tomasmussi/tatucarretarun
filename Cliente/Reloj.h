#ifndef __RELOJ_H__
#define __RELOJ_H__

#include <string>
#include <map>
#include <SDL.h>
#include "SDL_image.h"
#include <SDL_opengl.h>

class Reloj
{
	private:
	
		std::map<char,GLuint> texturas;
		void imprimirCaracter(char letra, float x, float y);
		void loadTexture(const char * path, char letra);
		
	public:
	
		void imprimirReloj(const std::string& reloj, float x, float y);
		void cargarTexturas();
	
};

#endif

