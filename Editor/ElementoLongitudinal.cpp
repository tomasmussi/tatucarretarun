#include "ElementoLongitudinal.h"
#include <math.h>
#include <iostream>
using namespace std;

ElementoLongitudinal::ElementoLongitudinal() {
}

ElementoLongitudinal::ElementoLongitudinal(const std::string & nombre,
		Coordenada posicion, const ulong magnitud, long angulo, ulong ancho) :
	Elemento(nombre, posicion, magnitud, angulo) {
	Coordenada p2;
	p2.setAngulo(angulo);
	p2.setModulo(magnitud);
	p2 = p2 + posicion;
	extremoOpuesto = p2;
	this->ancho = ancho;
	
}

ElementoLongitudinal::~ElementoLongitudinal() {
}


void ElementoLongitudinal::setPosicion(const Coordenada nuevaPosicion) {
	Elemento::setPosicion(nuevaPosicion);

	Coordenada p2;
	long ang = -getAngulo();
	
	p2.setX( cos(Angulo::aRadianes(ang))*getMagnitud() + getPosicion().x );
	p2.setY( sin(Angulo::aRadianes(ang))*getMagnitud() + getPosicion().y );
	extremoOpuesto = p2;
}

void ElementoLongitudinal::setAngulo(const long angulo) {
	Elemento::setAngulo(angulo);
}

void ElementoLongitudinal::actualizarPuntosDeEnlace(int modo) {
	
	int cantidadEnlaces = getMagnitud() / LONGITUD_DE_ENLACE_MINIMA;
	if(modo == 1) cantidadEnlaces = 1;

	int separacionEntreEnlaces = (cantidadEnlaces == 0) ? this->getMagnitud()
			: (getMagnitud() / cantidadEnlaces);
	
	eliminarEnlaces();
	_enlaces.clear();
	
	/* Completa la lista de coordenadas de enlace sobre el elemento longitudinal */
	for(ulong paso = 0; paso <= (this->getMagnitud()  - separacionEntreEnlaces ); paso += separacionEntreEnlaces) {
		Coordenada posicionEnlace;
		posicionEnlace.setModulo(paso); 
		posicionEnlace.setAngulo(-this->getAngulo());
		posicionEnlace = posicionEnlace + this->getPosicion();
		PuntoDeEnlace * nuevoEnlace = new PuntoDeEnlace(posicionEnlace, this);
		_enlaces.push_back(nuevoEnlace);
	}
	_enlaces.push_back(new PuntoDeEnlace(this->getExtremoOpuesto(), this));

}

void ElementoLongitudinal::eliminarEnlaces() {
	list <PuntoDeEnlace*>::iterator it;
	for(it = _enlaces.begin(); it != _enlaces.end(); it++) {
		delete (*it);
	}
	_enlaces.clear();
}

void ElementoLongitudinal::imprimirEnlaces() {
	list <PuntoDeEnlace*>::iterator it;
	for(it = _enlaces.begin(); it != _enlaces.end(); it++) {
		(*it)->getPosicion().imprimir();
	}
	cout << endl << endl;
}

std::list<PuntoDeEnlace*>  ElementoLongitudinal::getListaEnlaces() {
	return _enlaces;
}


ulong ElementoLongitudinal::getAncho() const {
	return ancho;
}


void ElementoLongitudinal::setExtremoOpuesto(const Coordenada p2) {
	
	extremoOpuesto = p2;

	Coordenada resta = p2 - getPosicion();
	setMagnitud(resta.modulo);
		
	 if (resta.angulo<180)
	 	resta.angulo=-resta.angulo;
	 	
	setAngulo(resta.angulo);

}


Coordenada ElementoLongitudinal::getExtremoOpuesto() const {
	return extremoOpuesto;
}

int ElementoLongitudinal::seleccionar(Coordenada punto) const {

	Coordenada aux = punto - this->getPosicion(); //traslado el punto al origen
	
	if (aux.angulo<180)
                aux.angulo=-aux.angulo;
	
	aux.setAngulo(aux.angulo - this->getAngulo()); //roto

	int resultado= ELEMENTO_NO_SELECCIONADO;

	/* Analisis de si se hizo click y en donde */
	if ( (aux.x <= this->getMagnitud()) && (fabs(aux.y) <= (this->ancho  + MARGEN_DE_AGARRE_ANCHO))) {
		resultado = ELEMENTO_SELECCIONADO_POR_CENTRO;
		if (aux.x > (long)(this->getMagnitud() - MARGEN_DE_AGARRE))
			resultado = ELEMENTO_SELECCIONADO_POR_EXTREMO2;
	}

	return resultado;
}

