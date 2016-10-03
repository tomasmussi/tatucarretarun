#include "Cinta.h"

Cinta::Cinta() :
	ElementoLongitudinal("Longitudinal:Cinta", Coordenada(long(10), long(10)), 25, 0, 10) {
}

Cinta::Cinta(Coordenada posicion, ulong magnitud, long angulo, long ancho) :

	ElementoLongitudinal("Longitudinal:Cinta", posicion, magnitud, angulo,
			ancho) {

}
Cinta::~Cinta() {
}
