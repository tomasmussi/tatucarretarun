#include "Ventana.h"
#include <iostream>
#include "Boton.h"
#include <vector>
#include "InterfazMapa.h"
#include "ParserXML.h"
#include "sms_app.h"
using namespace std;

Ventana::Ventana(int ancho, int alto) {

	this->alto = alto;
	this->ancho = ancho;
	this->salir = false;
	this->sms = false;

	int w = ancho;
	int h = alto;
	int depth = 16;

	SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	pantalla = SDL_SetVideoMode(w, h, depth, SDL_OPENGL | SDL_FULLSCREEN);
	SDL_WM_SetCaption("Editor de Escenarios", NULL);

	double ar = static_cast<double>(w) / h;
	glOrtho(-ar, ar, -1.0, 1.0, -1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		
}

Ventana::~Ventana() {

	 list<PanelContenedor*>::iterator it;
	 for (it = _paneles.begin(); it != _paneles.end(); it++) {
		 delete (*it);
	 }

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}

void Ventana::ejecutar() {

	SDL_Event eventos;
	
	Coordenada posicionMouse;

	
	while (!salir) {
		imprimir_pantalla();

		if (SDL_WaitEvent(&eventos)) {
			posicionMouse.setX(eventos.motion.x);
			posicionMouse.setY(eventos.motion.y);
			switch (eventos.type) {
			case SDL_QUIT:
				salir = true;
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				seEjecutaEventoEn(posicionMouse, SDL_MOUSEBUTTONDOWN);
				break;

			case SDL_MOUSEBUTTONUP:
				seEjecutaEventoEn(posicionMouse, SDL_MOUSEBUTTONUP);
				break;			
			
			case SDL_MOUSEMOTION:
				seEjecutaEventoEn(posicionMouse, SDL_MOUSEMOTION);
				break;


			default:
				break;
			}
		} else {
			cout << "Error " << SDL_GetError() << endl;
			salir = true;
		}
	}

}

void Ventana::agregarPanel(PanelContenedor * panel, const Coordenada posicion) {
	panel->setVentana(this);
	panel->setPantalla(pantalla);
	panel->setPosicion(posicion);
	panel->setId(_paneles.size() + 1);
	_paneles.push_back(panel);
}

void Ventana::agregarPanel(PanelContenedor * panel, const int lugar) {
	Coordenada pos;
	panel->setVentana(this);
	switch (lugar) {
	case 0:
		pos.setX(20 + _paneles.size() * (panel->getAncho() + 20));
		pos.setY(this->alto - panel->getAltura());
		break;
	case 1:
		pos.setX(0);
		pos.setY(this->alto - panel->getAltura());
	}

	panel->setPantalla(pantalla);
	panel->setPosicion(pos);
	panel->setId(_paneles.size() + 1);
	_paneles.push_back(panel);
}

void Ventana::imprimir_pantalla(void) {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	 list<PanelContenedor*>::iterator it;
	 for (it = _paneles.begin(); it != _paneles.end(); it++) {
			 (*it)->imprimir();
	 }
	 



	SDL_GL_SwapBuffers();
}

bool Ventana::seEjecutaEventoEn(const Coordenada punto, int evento) {
	list<PanelContenedor*>::iterator it;
	for (it = _paneles.begin(); it != _paneles.end(); it++) {
		if ((*it)->pertenece(punto)) {
			if ((*it)->seEjecutaEventoEn(punto , evento)) {
				return false;
			}
		}
	}

	switch(evento) {
		case SDL_MOUSEBUTTONDOWN: evento_MouseDown(punto); break;
		case SDL_MOUSEMOTION: evento_MouseOver(punto); break;
		case SDL_MOUSEBUTTONUP: evento_MouseUp(punto); break;
		
	}
		
	return true;
}

void Ventana::evento_MouseDown(Coordenada punto) {
	
}

void Ventana::evento_MouseOver(Coordenada punto) {

	list<PanelContenedor*>::iterator it;
	for (it = _paneles.begin(); it != _paneles.end(); it++)
		if((*it)->getNombre().find("Boton") != string::npos) {
			Boton * boton = (Boton*) (*it);
			boton->setMouseOver(false);
			if(boton->esta_presionado())
				boton->setImagenDeFondo(TEXTURA_MOUSEDOWN);
			else
				boton->setImagenDeFondo(TEXTURA_DEFECTO);
	}
}

void Ventana::evento_MouseUp(Coordenada punto) {
	
}

void Ventana::desclickearBotones(int id_excepcion) {
	list<PanelContenedor*>::iterator it;
	for (it = _paneles.begin(); it != _paneles.end(); it++)
		if((*it)->getNombre().find("Boton") != string::npos) {
			Boton * boton = (Boton*) (*it);
			if(boton->getId() == id_excepcion) {
				continue;
			} else {
				boton->desapretar();
				boton->setImagenDeFondo(TEXTURA_DEFECTO);
			}
		}
}

void Ventana::corregirCoordenadas(ulong *x, ulong *y) {
	*x = (*x - (ancho/2.0)) / 300;
	*y = (-*y + (alto/2.0)) / 300;

}


