#include "Reloj.h"
#include <iostream>
using namespace std;

#define TAM_LETRA_X 0.05
#define TAM_LETRA_Y 0.1

void Reloj::cargarTexturas()
{
	string path;
	
	char i;
/*	for (i = 'a' ; i <= 'z' ; i++)
	{
		path = "letras/letra__";
		path += i;
		path += ".bmp";
		loadTexture(path.c_str(),i);
	}*/
	for (i = '0' ; i <= '9' ; i++)
	{
		path = "letras/letra__";
		path += i;
		path += ".bmp";
		loadTexture(path.c_str(),i);
	}
	loadTexture("letras/letra__s.bmp",'s');
	loadTexture("letras/vacio.bmp",' ');
	loadTexture("letras/letra__dos_puntos.bmp",':');
	loadTexture("letras/letra__punto.bmp",'.');
	loadTexture("letras/letra__guion.bmp",'-');
	loadTexture("letras/letra__pesos.bmp",'$');

}

void Reloj::imprimirReloj(const std::string& reloj, float x, float y)
{	
	unsigned int i;
	
	for ( i = 0 ; i < reloj.length() ; i++)
	{
		imprimirCaracter(reloj[i],x,y);
		x += TAM_LETRA_X;
	}


}

void Reloj::imprimirCaracter(char letra, float x, float y)
{

	map<char,GLuint>::iterator it = texturas.find(letra);
	if (it == texturas.end())
		return;

	GLuint textura = it->second;

	glBindTexture( GL_TEXTURE_2D, textura);

	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( x, y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( x, y - TAM_LETRA_Y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( x + TAM_LETRA_X, y - TAM_LETRA_Y, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( x + TAM_LETRA_X, y, 0.0f );
	}
	glEnd();
}

void Reloj::loadTexture(const char * path, char letra) {
	GLuint texture;
	GLint nOfColors;
	GLenum texture_format = 0;
	SDL_Surface *surface;

	if ( (surface = SDL_LoadBMP(path)) ) { 

	// Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("ERROR en imagen, el ancho no es potencia de 2\n");
	}
	
	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("ERROR en imagen, el alto no es potencia de 2\n");
	}

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)     // contains an alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     // no alpha channel
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
     
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &texture );

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture );

	// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
        texturas.insert(make_pair(letra, texture));
} 
else {
	printf("SDL could not load image.bmp: %s\n", SDL_GetError());
	SDL_Quit();
}
}

