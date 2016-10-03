#include "Barra.h"

using namespace std;

Barra::Barra() :
	ElementoLongitudinal("Longitudinal:Barra", Coordenada(long(10), long(10)), 25, 0, 10) {
}

Barra::Barra(Coordenada posicion, ulong magnitud, long angulo, long ancho) :
	ElementoLongitudinal("Longitudinal:Barra", posicion, magnitud, angulo, ancho) {
}

Barra::~Barra() {
}

