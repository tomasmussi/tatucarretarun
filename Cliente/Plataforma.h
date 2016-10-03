#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

#include "ElementoLongitudinal.h"

class Plataforma : public ElementoLongitudinal
{
public:
	Plataforma();
	Plataforma(Coordenada posicion, ulong magnitud, long angulo, long ancho);
	virtual ~Plataforma();
};

#endif /*PLATAFORMA_H_*/
