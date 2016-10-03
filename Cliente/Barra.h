#ifndef BARRA_H_
#define BARRA_H_

#include "ElementoLongitudinal.h"


class Barra : public ElementoLongitudinal {
	
public:
	Barra();
	Barra(Coordenada posicion, ulong magnitud, long angulo, long ancho);
	virtual ~Barra();
};

#endif /*BARRA_H_*/
