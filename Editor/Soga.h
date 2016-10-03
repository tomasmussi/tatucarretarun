#ifndef SOGA_H_
#define SOGA_H_

#include "ElementoLongitudinal.h"

class Soga : public ElementoLongitudinal
{
public:
	Soga();
	Soga(Coordenada posicion, ulong magnitud, long angulo, long ancho);
	virtual ~Soga();
};

#endif /*SOGA_H_*/
