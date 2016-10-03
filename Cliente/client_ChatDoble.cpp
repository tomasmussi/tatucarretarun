#include "client_ChatDoble.h"
#include <iostream>
#include <fstream>
using namespace std;


#define INICIO_CHAT_X 0.965
#define INICIO_CHAT_Y 0.1
#define TAM_LETRA_X 0.025
#define TAM_LETRA_Y 0.05

#define INICIO_FONDO_X 0.9
#define INICIO_FONDO_Y 0.3
#define TAM_FONDO_X 0.75
#define TAM_FONDO_Y 1.2

#define INICIO_FONDO_RIVAL_X 0.57
#define INICIO_FONDO_RIVAL_Y 1.0
#define TAM_FONDO_RIVAL_X 1.1
#define TAM_FONDO_RIVAL_Y 0.7
#define ANCHO_LINEA 0.005

ChatDoble::ChatDoble()
{
	visible = true;
	salir = false;
	nuevoMovimiento = false;
	simular = false;
	bloquear = false;
}

bool ChatDoble::debeSimular()
{
	bool retorno;
	m.lock();
	retorno = simular;
	m.unlock();
	return retorno;
}

void ChatDoble::setSimular(bool valor)
{
	m.lock();
	simular = valor;
	m.unlock();
}

bool ChatDoble::debeBloquear()
{
	bool retorno;
	m.lock();
	retorno = bloquear;
	m.unlock();
	return retorno;
}

void ChatDoble::setBloquear(bool valor)
{
	m.lock();
	bloquear = valor;
	m.unlock();
}

void ChatDoble::cargarTexturas()
{
	
	string path;
	
	char i;
	// carga texturas minúsculas
	for (i = 'a' ; i <= 'z' ; i++)
	{
		path = "letras/letra__";
		path += i;
		path += ".bmp";
		loadTexture(path.c_str(),i);
	}
	// carga texturas mayúsculas
	for (i = 'A' ; i <= 'Z' ; i++)
	{
		path = "letras/letra__";
		path += i;
		path += ".bmp";
		loadTexture(path.c_str(),i);
	}
	// carga texturas números
	for (i = '0' ; i <= '9' ; i++)
	{
		path = "letras/letra__";
		path += i;
		path += ".bmp";
		loadTexture(path.c_str(),i);
	}
	// carga caracteres especiales
	loadTexture("letras/vacio.bmp",' ');
	loadTexture("letras/letra__dos_puntos.bmp",':');
	loadTexture("letras/letra__punto.bmp",'.');
	loadTexture("letras/letra__coma.bmp",',');
	loadTexture("letras/letra__admiracion.bmp",'!');
	loadTexture("letras/letra__interrogacion.bmp",'?');
	loadTexture("letras/letra__admiracion_bajo.bmp",(char)161);
	loadTexture("letras/letra__amp.bmp",'&');
	loadTexture("letras/letra__arroba.bmp",'@');
	loadTexture("letras/letra__asterisco.bmp",'*');
	loadTexture("letras/letra__barra.bmp",'/');
	loadTexture("letras/letra__comillas.bmp",'"');
	loadTexture("letras/letra__guion.bmp",'-');
	loadTexture("letras/letra__guion_bajo.bmp",'_');
	loadTexture("letras/letra__interrogacion_bajo.bmp",(char)191);
	loadTexture("letras/letra__not.bmp",(char)172);
	loadTexture("letras/letra__numeral.bmp",'#');
	loadTexture("letras/letra__parentesis_izq.bmp",'(');
	loadTexture("letras/letra__parentesis_der.bmp",')');
	loadTexture("letras/letra__pesos.bmp",'$');
	loadTexture("letras/letra__porcentaje.bmp",'%');
	
	//carga fondos y cursor
	loadTexture("negro.bmp",'+');
	loadTexture("letras/cursor.bmp",'=');
	loadTexture("letras/fondo.png",'>');
}

bool ChatDoble::agregarLetra(char letra)
{
	if (buffer.length() == MAX_CARACTERES)
		return false; // no hay más lugar
		
	buffer += letra;
	return true;
}
		

bool ChatDoble::quitarLetra()
{
	if (buffer.length() == 0)
		return false;

	buffer = buffer.substr(0, buffer.length() - 1); //borra una letra
	return true;
}
		

void ChatDoble::agregarLinea(const std::string& linea)
{
	if (lineas.size() == MAX_LINEAS)
		lineas.erase(lineas.begin());
	lineas.push_back(linea);
	
}
		
		
void ChatDoble::imprimirChat()
{	

	if (!visible)
		return;

	imprimirFondo();

	unsigned int i;
	string linea;
	float x = INICIO_CHAT_X;
	float y = INICIO_CHAT_Y;
	
	m.lock();
	
	// imprime líneas
	vector<string>::iterator itLineas = lineas.begin();
	while (itLineas != lineas.end())
	{
		linea = *itLineas;
		for ( i = 0 ; i < linea.length() ; i++)
		{
			imprimirCaracter(linea[i],x,y);
			x += TAM_LETRA_X;
		}
		x = INICIO_CHAT_X;
		y -= TAM_LETRA_Y;
		itLineas++;
	}
	// imprime buffer
	y = INICIO_CHAT_Y - (MAX_LINEAS+3) * TAM_LETRA_Y;	
	for ( i = 0 ; i < buffer.length() ; i++)
	{
		imprimirCaracter(buffer[i],x,y);
		x += TAM_LETRA_X;
	}
	imprimirCaracter('=',x,y);
	
	m.unlock();
}

void ChatDoble::imprimirFondoRival()
{
	map<char,GLuint>::iterator it = texturas.find('+');
	if (it == texturas.end())
		return;

	GLuint textura = it->second;

	glBindTexture( GL_TEXTURE_2D, textura);
	
	//Linea izquierda
	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y, 0.0f );
	}
	glEnd();
	
	
	//Linea derecha
	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X + TAM_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X + TAM_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y, 0.0f );
	}
	glEnd();

	
	//Linea arriba
	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y - ANCHO_LINEA, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y - ANCHO_LINEA, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y, 0.0f );
	}
	glEnd();
	
	
	//Linea abajo
	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y - ANCHO_LINEA, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y - ANCHO_LINEA, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( INICIO_FONDO_RIVAL_X  + TAM_FONDO_RIVAL_X + ANCHO_LINEA, INICIO_FONDO_RIVAL_Y - TAM_FONDO_RIVAL_Y, 0.0f );
	}
	glEnd();

}

void ChatDoble::imprimirFondo()
{
	map<char,GLuint>::iterator it = texturas.find('>');
	if (it == texturas.end())
		return;

	GLuint textura = it->second;

	glBindTexture( GL_TEXTURE_2D, textura);

	glBegin(GL_QUADS);
	{
		glTexCoord2i( 0, 0 );
		glVertex3f( INICIO_FONDO_X, INICIO_FONDO_Y, 0.0f );
		glTexCoord2i( 0, 1 );
		glVertex3f( INICIO_FONDO_X, INICIO_FONDO_Y - TAM_FONDO_Y, 0.0f );
		glTexCoord2i( 1, 1 );
		glVertex3f( INICIO_FONDO_X + TAM_FONDO_X, INICIO_FONDO_Y - TAM_FONDO_Y, 0.0f );
		glTexCoord2i( 1, 0 );
		glVertex3f( INICIO_FONDO_X + TAM_FONDO_X, INICIO_FONDO_Y, 0.0f );
	}
	glEnd();
	
	imprimirFondoRival();

	
}

void ChatDoble::imprimirCaracter(char letra, float x, float y)
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

bool ChatDoble::enter()
{
	if (buffer.length() == 0)
		return false;
		
	m.lock();
	entradas.push_back("C"+buffer);
	m.unlock();

	buffer.clear();
	
	return true;
}

void ChatDoble::agregarMensaje(const std::string& mensaje)
{
	m.lock();
	entradas.push_back(mensaje);
	m.unlock();
}

bool ChatDoble::obtenerEntrada(std::string& entrada)
{
	bool retorno = true;
	m.lock();
	if (entradas.empty())
		retorno = false;
	else
	{
		entrada = entradas[0];
		entradas.erase(entradas.begin());
	}
	m.unlock();
	return retorno;
}

char ChatDoble::esLetra(SDL_Event evento)
{

	if (((evento.key.keysym.unicode >= 'a') && (evento.key.keysym.unicode <= 'z'))
	     ||
	   ((evento.key.keysym.unicode >= 'A') && (evento.key.keysym.unicode <= 'Z'))
	     ||
	   ((evento.key.keysym.unicode >= '0') && (evento.key.keysym.unicode <= '9'))
	   || (evento.key.keysym.unicode == '"') || (evento.key.keysym.unicode == '#')
	   || (evento.key.keysym.unicode == '%') || (evento.key.keysym.unicode == '$')
	   || (evento.key.keysym.unicode == '/') || (evento.key.keysym.unicode == 172)
	   || (evento.key.keysym.unicode == '.') || (evento.key.keysym.unicode == ':')
	   || (evento.key.keysym.unicode == '-') || (evento.key.keysym.unicode == ',')
	   || (evento.key.keysym.unicode == '_') || (evento.key.keysym.unicode == '(')
	   || (evento.key.keysym.unicode == '*') || (evento.key.keysym.unicode == ')')
	   || (evento.key.keysym.unicode == '@') || (evento.key.keysym.unicode == '&')
	   || (evento.key.keysym.unicode == 161) || (evento.key.keysym.unicode == 191) || (evento.key.keysym.unicode == ' ')
	   || (evento.key.keysym.unicode == '!') || (evento.key.keysym.unicode == '?'))

		return evento.key.keysym.unicode;

	
	return -1;
}

void ChatDoble::loadTexture(const char * path, char letra) {
	GLuint texture;
	GLint nOfColors;
	GLenum texture_format = 0;
	SDL_Surface *surface;

	if ( (surface = IMG_Load(path)) ) {  

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
                printf("ERROR truecolor\n");

        }
     
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels );
        texturas.insert(make_pair(letra, texture));
} 
else {
	printf("SDL no pudo abrir imagen: %s\n", SDL_GetError());
	SDL_Quit();
}

}

void ChatDoble::cambiarVisible()
{
	visible = !visible;
}

bool ChatDoble::debeSalir()
{
	bool retorno;
	m.lock();
	retorno = salir;
	m.unlock();
	return retorno;
}
		
void ChatDoble::setSalir(bool valor)
{
	m.lock();
	salir = valor;
	m.unlock();
}

void ChatDoble::cambiarMovimiento(const std::string& xml)
{
	m.lock();
	nuevoMovimiento = true;
	movimiento = xml;
	m.unlock();
}

bool ChatDoble::obtenerMovimiento(std::string& xml)
{
	bool retorno = true;
	m.lock();
	if (nuevoMovimiento)
	{
		xml = movimiento;
		nuevoMovimiento = false;
	}
	else
		retorno = false;
	m.unlock();
	
	return retorno;
}



