#include "Plataforma.h"

Plataforma::Plataforma() :
	ElementoLongitudinal("Longitudinal:Plataforma", Coordenada(long(10), long(10)), 25, 0, 10) {

}

Plataforma::Plataforma(Coordenada posicion, ulong magnitud, long angulo, long ancho) :
	ElementoLongitudinal("Longitudinal:Plataforma", posicion, magnitud, angulo, ancho) {
}

Plataforma::~Plataforma() {
}
