#include "PuntoFijo.h"

PuntoFijo::PuntoFijo() :
	ElementoEnlazable("Circular:Enlazable:PuntoFijo", Coordenada(long(100), long(100)), 10, 0, NULL) {
}

PuntoFijo::PuntoFijo(Coordenada posicion, ulong magnitud) :
	ElementoEnlazable("Circular:Enlazable:PuntoFijo", posicion, magnitud, 0, NULL) {
	
}
PuntoFijo::~PuntoFijo() { 
}
