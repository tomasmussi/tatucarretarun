#ifndef ELEMENTOCIRCULAR_H_
#define ELEMENTOCIRCULAR_H_

#include "Elemento.h"
#include <string>

class ElementoCircular : public Elemento {

public:
	ElementoCircular();
	ElementoCircular(const std::string & nombre, Coordenada posicion,
			const ulong magnitud, long angulo);
	virtual ~ElementoCircular();

	/* Devuelve true si la Coordenada pertenece al Elemento */
	virtual int seleccionar(Coordenada) const;
};

#endif /*ELEMENTOCIRCULAR_H_*/
