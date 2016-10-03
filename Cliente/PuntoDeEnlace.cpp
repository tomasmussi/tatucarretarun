#include "PuntoDeEnlace.h"

PuntoDeEnlace::PuntoDeEnlace() :
	ElementoEnlazable("Circular:Enlazable:PuntoDeEnlace", Coordenada(long(100), long(100)), 15, 0, NULL) {
}


PuntoDeEnlace::PuntoDeEnlace(Coordenada posicion,  Elemento * poseedor) :
	ElementoEnlazable("Circular:Enlazable:PuntoDeEnlace", posicion, 10, 0, poseedor) {
}

PuntoDeEnlace::~PuntoDeEnlace() {
}
