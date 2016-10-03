#ifndef LLEGADA_H_
#define LLEGADA_H_

#include <iostream>
#include "Coordenada.h"
#include "constantes.h"
#include "Elemento.h"

class Llegada : public Elemento {

public:
	Llegada(Coordenada);
	int seleccionar(Coordenada) const;


};

#endif /*LLEGADA_H_*/
