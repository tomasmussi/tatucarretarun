#include "Soga.h"

Soga::Soga() :
	ElementoLongitudinal("Longitudinal:Soga", Coordenada(long(10), long(10)), 25, 0, 10) {
}

Soga::Soga(Coordenada posicion, ulong magnitud, long angulo, long ancho) :
	ElementoLongitudinal("Longitudinal:Soga", posicion, magnitud, angulo, ancho) {
}

Soga::~Soga()
{
}
