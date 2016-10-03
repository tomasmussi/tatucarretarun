#include "video.h"
#include <SDL.h>
#include <SDL_opengl.h>

void Video::loadSurface(char* surf){
	if ( (surface = SDL_LoadBMP(surf)) ) { 
 
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
} 
else {
	printf("SDL could not load image.bmp: %s\n", SDL_GetError());
	SDL_Quit();
	
}    

if ( surface ) { 
	SDL_FreeSurface( surface );
}
	textures.push_back(texture);

}

void Video::loadSurfaces(){
	loadSurface("ima/mapa/ball.bmp"); //0
	loadSurface("ima/mapa/punto_fijo.bmp"); //1
	loadSurface("ima/mapa/rocket.bmp"); //2
	loadSurface("ima/mapa/metal.bmp");//3
	loadSurface("ima/mapa/platform.bmp");//4
	loadSurface("ima/mapa/rope.bmp"); //5
	loadSurface("ima/mapa/lona.bmp"); //6
	loadSurface("ima/mapa/rocket.bmp"); //7
	loadSurface("ima/mapa/llegada.bmp"); // 8
	loadSurface("ima/mapa/ballbright.bmp"); //9
		


}
Video::Video(const Config& config)
{
	int w = config.read<int>("Video.Width");
	int h = config.read<int>("Video.Height");
	int depth = config.read<int>("Video.Depth");
	
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(w, h, depth, SDL_OPENGL | SDL_FULLSCREEN);

	SDL_WM_SetCaption("Tatu Carreta RUN", 0);


	double ar = static_cast<double>(w) / h;
	glOrtho(-ar, ar, -1.0, 1.0, -1.0, 1.0);
	glEnable( GL_TEXTURE_2D );
 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
 
//	glViewport( 0, 0, w, h);
 
	glClear( GL_COLOR_BUFFER_BIT );
 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
 
	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	loadSurfaces();
       glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}

Video::~Video()
{
	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
