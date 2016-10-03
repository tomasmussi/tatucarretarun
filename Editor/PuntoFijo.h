#ifndef PUNTOFIJO_H_
#define PUNTOFIJO_H_

#include "ElementoEnlazable.h"

class PuntoFijo : public ElementoEnlazable
{
public:
	PuntoFijo();
	PuntoFijo(Coordenada posicion, ulong magnitud);
	virtual ~PuntoFijo();
};

#endif /*PUNTOFIJO_H_*/
