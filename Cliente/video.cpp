#include "video.h"


void Video::loadSurface(const char* surf){
	GLuint texture;		
	SDL_Surface *surface;	 
	GLenum texture_format=0;
	GLint  nOfColors;
	if ( (surface =  IMG_Load(surf)) ) { 
 
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("ERROR en imagen, el ancho no es potencia de 2\n");
	}
	
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("ERROR en imagen, el alto no es potencia de 2\n");
	}
 
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4)    
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGBA;
                else
                        texture_format = GL_BGRA;
        } else if (nOfColors == 3)     
        {
                if (surface->format->Rmask == 0x000000ff)
                        texture_format = GL_RGB;
                else
                        texture_format = GL_BGR;
        } else {
                printf("Atención: la imagen no es truecolor\n");

        }
        
	glGenTextures( 1, &texture );
 
	glBindTexture( GL_TEXTURE_2D, texture );
 
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
} 
else {
	printf("No se pudo cargar la imagen: %s\n", SDL_GetError());
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
	loadSurface(pathFondo); //10
	fondo = textures.at(10);


}
Video::Video(const Config& config, const char* pathFondo)
{
	this->pathFondo = pathFondo;
	int w = config.read<int>("Video.Width");
	int h = config.read<int>("Video.Height");
	int depth = config.read<int>("Video.Depth");
	
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(w, h, depth, SDL_OPENGL); //| SDL_FULLSCREEN);

	SDL_WM_SetCaption("Tatu Carreta RUN", 0);


	double ar = static_cast<double>(w) / h;
	glOrtho(-ar, ar, -1.0, 1.0, -1.0, 1.0);
	glEnable( GL_TEXTURE_2D );
 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	
	glClear( GL_COLOR_BUFFER_BIT );
 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	loadSurfaces();
       glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}

