#include "ElementoCircular.h"

ElementoCircular::ElementoCircular(const std::string & nombre,
		Coordenada posicion, const ulong magnitud, long angulo) :
	Elemento(nombre, posicion, magnitud, angulo) {

}

ElementoCircular::ElementoCircular() {
}

ElementoCircular::~ElementoCircular() {
}

int ElementoCircular::seleccionar(Coordenada punto) const {
	int seleccion= ELEMENTO_NO_SELECCIONADO;
	Coordenada resta = punto - getPosicion();

	if (resta.modulo <= getMagnitud()) {
		seleccion = ELEMENTO_SELECCIONADO_POR_CENTRO;
	}

	return seleccion;
}
