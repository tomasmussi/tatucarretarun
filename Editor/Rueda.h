#ifndef RUEDA_H_
#define RUEDA_H_

#include "ElementoCircular.h"
#include <list>
#include "PuntoDeEnlace.h"

class Rueda : public ElementoCircular {
private:
	std::list <PuntoDeEnlace*> _enlaces;

public:
	/* Constructor por defecto */
	Rueda();
	/* Constructor con parametros */
	Rueda(Coordenada posicion, ulong magnitud);
	/* Destructor virtual */
	virtual ~Rueda();
	/* Ubica los enlaces */
	void actualizarPuntosDeEnlace();
	/* Imprime los enlaces */
	void imprimirEnlaces();
	/* Devuelve puntero a lista de enlaces */
	std::list<PuntoDeEnlace*> getListaEnlaces();
	/* Elimina los enlaces */
	void eliminarEnlaces();

};

#endif /*RUEDA_H_*/
