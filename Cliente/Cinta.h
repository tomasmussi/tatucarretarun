#ifndef CINTA_H_
#define CINTA_H_

#include "ElementoLongitudinal.h"

class Cinta : public ElementoLongitudinal {
public:
	Cinta();
	Cinta(Coordenada posicion, ulong magnitud, long angulo, long ancho);
	virtual ~Cinta();
};

#endif /*CINTA_H_*/
