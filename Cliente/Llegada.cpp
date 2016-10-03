#include "Llegada.h"

Llegada::Llegada(Coordenada posicion) : Elemento ("ZonaLlegada", posicion, 80, 0) {
	
}


int Llegada::seleccionar(Coordenada punto) const {

	if((punto.x > getPosicion().x) && (punto.x < (getPosicion().x + (long)getMagnitud())))
		if((punto.y > getPosicion().y) && (punto.y < (getPosicion().y + (long)getMagnitud())))
			return ELEMENTO_SELECCIONADO_POR_CENTRO;
	return ELEMENTO_NO_SELECCIONADO;
}


