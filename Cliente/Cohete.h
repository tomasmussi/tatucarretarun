#ifndef COHETE_H_
#define COHETE_H_

#include "ElementoLongitudinal.h"

class Cohete : public ElementoLongitudinal
{
public:
	Cohete();
	Cohete(Coordenada posicion, ulong magnitud, long angulo, long ancho);
	virtual ~Cohete();
};

#endif /*COHETE_H_*/
