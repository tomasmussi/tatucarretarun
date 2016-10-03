#ifndef PUNTODEENLACE_H_
#define PUNTODEENLACE_H_

#include "ElementoEnlazable.h"

class PuntoDeEnlace : public ElementoEnlazable
{
public:
	PuntoDeEnlace();
	PuntoDeEnlace(Coordenada posicion, Elemento * poseedor);
	virtual ~PuntoDeEnlace();
};

#endif /*PUNTODEENLACE_H_*/
