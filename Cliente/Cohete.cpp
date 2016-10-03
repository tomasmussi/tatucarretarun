#include "Cohete.h"

Cohete::Cohete() :
	ElementoLongitudinal("Longitudinal:Cohete", Coordenada(long(10), long(10)), 25, 0, 10) {
}

Cohete::Cohete(Coordenada posicion, ulong magnitud, long angulo, long ancho) :
	ElementoLongitudinal("Longitudinal:Cohete", posicion, magnitud, angulo,
			ancho) {

}

Cohete::~Cohete() {
}
