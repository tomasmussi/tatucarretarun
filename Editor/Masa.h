#ifndef MASA_H_
#define MASA_H_

#include "ElementoCircular.h"

class Masa : public ElementoCircular {
public:
	/* Constructor por defecto */
	Masa();
	/* Constructor con parametros */
	Masa(Coordenada posicion, ulong magnitud);
	/* Destructor virtual */
	virtual ~Masa();

};

#endif /*MASA_H_*/
