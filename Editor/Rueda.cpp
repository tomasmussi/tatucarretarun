#include "Rueda.h"
#include <math.h>
using namespace std;

Rueda::Rueda() :
	ElementoCircular("Circular:Rueda", Coordenada(long(100), long(100)), 10, 0) {
}

Rueda::Rueda(Coordenada posicion, ulong magnitud) :
	ElementoCircular("Circular:Rueda", posicion, magnitud, 0) {
}

Rueda::~Rueda() {
	eliminarEnlaces();
	_enlaces.clear();
}

void Rueda::actualizarPuntosDeEnlace() {
	eliminarEnlaces();
	_enlaces.clear();

	ulong radio = getMagnitud();
	ulong x = getPosicion().x;
	ulong y = getPosicion().y;
	
	for (int j = 0; j <= POLY_SIDES; j += 2) {
		Coordenada pos;
		pos.setX(x + radio * cos(j * (2 * PI
				/ POLY_SIDES)));
		pos.setY(y + radio * sin(j * (2 * PI
				/ POLY_SIDES)));
		
		_enlaces.push_back(new PuntoDeEnlace(pos, this));
	
	}
}


void Rueda::eliminarEnlaces() {
	list <PuntoDeEnlace*>::iterator it;
	for(it = _enlaces.begin(); it != _enlaces.end(); it++) {
		delete (*it);
	}
	_enlaces.clear();
}

void Rueda::imprimirEnlaces() {
	list <PuntoDeEnlace*>::iterator it;
	cout << "Cantidad de enlaces: " << _enlaces.size() << endl;
	for(it = _enlaces.begin(); it != _enlaces.end(); it++) {
		(*it)->getPosicion().imprimir();
	}
	cout << endl << endl;
}

std::list<PuntoDeEnlace*>  Rueda::getListaEnlaces() {
	return _enlaces;
}

