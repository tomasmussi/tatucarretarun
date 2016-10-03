#include "FabricaDeElementos.h"

FabricaDeElementos::FabricaDeElementos() {
}

FabricaDeElementos::~FabricaDeElementos() {
}

Elemento * FabricaDeElementos::crear(int nroElemento, Coordenada posicion) {
	switch (nroElemento) {
	case MASA:
		return new Masa(posicion, 20);
	case RUEDA:
		return new Rueda(posicion, 30);
	case PUNTO_FIJO:
		return new PuntoFijo(posicion, 25);
	case BARRA:
		return new Barra(posicion, LONGITUD_MINIMA, 0, 5);
	case PLATAFORMA:
		return new Plataforma(posicion, LONGITUD_MINIMA, 0, 6);
	case SOGA:
		return new Soga(posicion, LONGITUD_MINIMA, 0, 5);
	case CINTA:
		return new Cinta(posicion, LONGITUD_MINIMA, 0, 5);
	case COHETE:
		return new Cohete(posicion, LONGITUD_COHETE , 0, 20);
	case ZONA_LLEGADA:
		return new Llegada(posicion); 
	}
	return NULL;
}

