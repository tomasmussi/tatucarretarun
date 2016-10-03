#ifndef FABRICADEELEMENTOS_H_
#define FABRICADEELEMENTOS_H_

#include "Coordenada.h"
#include "Elemento.h"
#include "Masa.h"
#include "Rueda.h"
#include "PuntoFijo.h"
#include "Barra.h"
#include "Soga.h"
#include "Plataforma.h"
#include "Cinta.h"
#include "Cohete.h"
#include "constantes.h"
#include "Llegada.h"



class FabricaDeElementos
{
public:
	FabricaDeElementos();
	virtual ~FabricaDeElementos();
	Elemento * crear (int nroElemento, Coordenada posicion);
};

#endif /*FABRICADEELEMENTOS_H_*/
