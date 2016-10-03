#include "PanelContenedor.h"
#include <iostream>
#include "constantes.h"
using namespace std;
PanelContenedor::PanelContenedor(const string & nombre,
		const string & rutaImgDefecto, const string & rutaImgClickDown,
		const string & rutaImgMouseOver) {

	this->nombre = nombre;

	ancho = getAncho(rutaImgDefecto);
	alto = getAlto(rutaImgDefecto);

	loadTexture(rutaImgDefecto.c_str(), TEXTURA_DEFECTO);
	loadTexture(rutaImgClickDown.c_str(), TEXTURA_MOUSEDOWN);
	loadTexture(rutaImgMouseOver.c_str(), TEXTURA_MOUSEOVER);
	loadTexture(rutaImgMouseOver.c_str(), TEXTURA_INHABILITADO);
	texturaActual = TEXTURA_DEFECTO;

	this->id = 0;

	posicion.setX(0);
	posicion.setY(0);
	pantalla = NULL;
	visible = true;
	habilitado = true;
	clicked = false;
	mouseOver = false;
	TX = 300.0;
	TY = 300.0;
}

PanelContenedor::PanelContenedor(const string & nombre,
		const string & rutaImgDefecto, const string & rutaImgClickDown,
		const string & rutaImgMouseOver, double zoom) {

	this->nombre = nombre;

	ancho = getAncho(rutaImgDefecto);
	alto = getAlto(rutaImgDefecto);

	loadTexture(rutaImgDefecto.c_str(), TEXTURA_DEFECTO);
	loadTexture(rutaImgClickDown.c_str(), TEXTURA_MOUSEDOWN);
	loadTexture(rutaImgMouseOver.c_str(), TEXTURA_MOUSEOVER);
	loadTexture(rutaImgMouseOver.c_str(), TEXTURA_INHABILITADO);
	texturaActual = TEXTURA_DEFECTO;

	this->id = 0;

	posicion.setX(0);
	posicion.setY(0);
	pantalla = NULL;
	visible = true;
	habilitado = true;
	clicked = false;
	mouseOver = false;
	TX = 300.0 / zoom;
	TY = 300.0 / zoom;
}

ulong PanelContenedor::getAncho(const string & rutaImg) {
	SDL_Surface * surface = IMG_Load(rutaImg.c_str());
	return (ulong) surface->w;
}

ulong PanelContenedor::getAlto(const string & rutaImg) {
	SDL_Surface * surface = IMG_Load(rutaImg.c_str());
	return (ulong) surface->h;
}

void PanelContenedor::loadTexture(const char * path, int tipoTextura) {
	GLuint textura;
	GLint nOfColors;
	GLenum texture_format = 0;

	SDL_Surface * surface = IMG_Load(path);

	if (surface == NULL) {
		cout << "No se pudo abrir la imagen: " << path << endl;
		return;
	}

	if ( (surface->w & (surface->w - 1)) != 0) {
		printf("ERROR en imagen, el ancho no es potencia de 2\n");
	}

	if ( (surface->h & (surface->h - 1)) != 0) {
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
		printf("warning: the image is not truecolor..  this will probably break\n");
	}

	glGenTextures( 1, &textura);

	glBindTexture( GL_TEXTURE_2D, textura);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
	texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	_texturas.insert(pair<int, GLuint>(tipoTextura, textura));

}

PanelContenedor::~PanelContenedor() {
}

double PanelContenedor::cambioX(long x) {
	return (x - pantalla->w/2.0)/TX;
}

double PanelContenedor::cambioY(long y) {
	return (-y + pantalla->h/2.0)/TY;
}

void PanelContenedor::imprimirImagenDeFondo() {

	glBindTexture( GL_TEXTURE_2D, _texturas[texturaActual]);
	glBegin(GL_QUADS);

	glTexCoord2i( 0, 0);
	glVertex3f(cambioX(posicion.x), cambioY(posicion.y), 0.0f);
	glTexCoord2i( 1, 0);
	glVertex3f(cambioX(posicion.x), cambioY(posicion.y + alto), 0.0f);
	glTexCoord2i( 1, 1);
	glVertex3f(cambioX(posicion.x + ancho), cambioY(posicion.y + alto), 0.0f);
	glTexCoord2i( 0, 1);
	glVertex3f(cambioX(posicion.x + ancho), cambioY(posicion.y), 0.0f);

	glEnd();

}

void PanelContenedor::imprimir() {
	imprimirImagenDeFondo();
	list <PanelContenedor*>::iterator it;

	for (it = _paneles.begin(); it != _paneles.end(); it++) {
		(*it)->imprimir();
	}
}

void PanelContenedor::setAltura(const ulong alto) {
	this->alto = alto;
}

void PanelContenedor::setAncho(const ulong ancho) {
	this->ancho = ancho;
}

ulong PanelContenedor::getAltura() const {
	return alto;
}

ulong PanelContenedor::getAncho() const {
	return ancho;
}

void PanelContenedor::setPosicion(const Coordenada posicion) {
	this->posicion = posicion;
}

Coordenada PanelContenedor::getPosicion() const {
	return posicion;
}

void PanelContenedor::setPantalla(SDL_Surface * pantalla) {
	this->pantalla = pantalla;
}

void PanelContenedor::setVentana(Ventana * ventana) {
	this->ventana = ventana;
}

string PanelContenedor::getNombre() const {
	return nombre;
}

int PanelContenedor::getId() const {
	return id;
}

void PanelContenedor::setId(const int id) {
	this->id = id;
}

void PanelContenedor::setMouseOver(const bool b) {
	this->mouseOver = b;
}

void PanelContenedor::agregarPanel(PanelContenedor * panel,
		Coordenada posicionEnPanel) {
	panel->setPosicion(this->posicion + posicionEnPanel);
	panel->setPantalla(pantalla);
	_paneles.push_back(panel);
}

void PanelContenedor::agregarPanel(PanelContenedor * panel, int lugar) {
	Coordenada pos;
	_paneles.push_back(panel);
	switch (lugar) {
	case 1:
		pos.setX(this->ancho / _paneles.size());
		pos.setY(this->alto + ((this->alto - panel->getAltura())/2));
		break;
	}

	panel->setPantalla(pantalla);
	panel->setPosicion(pos);

}

void PanelContenedor::setImagenDeFondo(int imagen) {
	texturaActual = imagen;
}

bool PanelContenedor::pertenece(Coordenada punto) {

	int bordeIzquierdo = posicion.x;
	int bordeDerecho = posicion.x + ancho;
	int bordeSuperior = posicion.y;
	int bordeInferior = posicion.y + alto;

	if ( (punto.x >= bordeIzquierdo) && (punto.x < bordeDerecho) && (punto.y
			> bordeSuperior) && (punto.y < bordeInferior))
		return true;
	return false;
}

bool PanelContenedor::seEjecutaEventoEn(const Coordenada punto, int evento) {

	if (!pertenece(punto)) {
		return false;
	}

	list<PanelContenedor*>::iterator it;
	for (it = _paneles.begin(); it != _paneles.end(); it++)
		if ((*it)->pertenece(punto))
			if ((*it)->seEjecutaEventoEn(punto, evento))
				return false;
if(habilitado) {
	switch (evento) {
	case SDL_MOUSEBUTTONDOWN:
		evento_MouseDown(punto - getPosicion());
		break;
	case SDL_MOUSEMOTION:
		evento_MouseOver(punto - getPosicion());
		break;
	case SDL_MOUSEBUTTONUP:
		evento_MouseUp(punto - getPosicion());
		break;
	}
	return true;
}
	return false;
}

void PanelContenedor::evento_MouseDown(Coordenada punto) {
	if (clicked==false) {
		clicked = true;
		if (texturaActual != TEXTURA_MOUSEDOWN)
			texturaActual = TEXTURA_MOUSEDOWN;
		else
			texturaActual = TEXTURA_DEFECTO;
	}
	

}

void PanelContenedor::evento_MouseOver(Coordenada punto) {

	if (mouseOver == false) {
		mouseOver = true;
		texturaActual = TEXTURA_MOUSEOVER;
	}

}

void PanelContenedor::evento_MouseUp(Coordenada punto) {
	if (clicked == true) {
		clicked = false;
	}
}

bool PanelContenedor::is_clicked() const {
	return clicked;
}

bool PanelContenedor::is_over() const {
	return mouseOver;
}

bool PanelContenedor::estaHabilitado() const {
	return habilitado;
}

void PanelContenedor::setHabilitar(const bool b) {
	habilitado = b;
}

