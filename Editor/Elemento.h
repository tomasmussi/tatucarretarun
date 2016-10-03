#ifndef ELEMENTO_H_
#define ELEMENTO_H_

#include <iostream>
#include "Coordenada.h"
#include <sstream>
#include "constantes.h"



typedef unsigned long ulong;

class Elemento {
private:

	ulong magnitud; /* Magnitud del elemento. Se adecua al tipo de elemento */
	long angulo; /* Angulo del elemento */
	Coordenada posicion; /* Posicion del punto representativo del elemento */
	bool bloqueado; /* Indica si el elemento puede ser modificado */
	std::string nombre; /* Nombre y jerarquía del elemento */
	bool mostrarEnlaces; /* Si debe mostrar sus enlaces */

public:
	/* Contructor vacio */
	Elemento();
	/* Contructor con paramatros */
	Elemento(const std::string & nombre, Coordenada posicion,
			const ulong magnitud, long angulo);
	/* Contructor virtual */
	virtual ~Elemento();

	/* Redimensiona la longitud en deltaUnidades. */
	void setMagnitud(const long valor);
	/* Devuelve el valor de la magnitud del elemento */
	ulong getMagnitud() const;
	/* Da un valor al angulo */
	virtual void setAngulo(const long);
	/* Devuelve el angulo */
	long getAngulo() const;
	/* Guarda la posicion en la que se encuentra el Elemento */
	virtual void setPosicion(const Coordenada nuevaPosicion);
	/* Devuelve la posicion en la que se encuentra el Elemento */
	Coordenada getPosicion() const;
	/* Devuelve el extremo opueso calculado por trigonometria */
	virtual Coordenada getExtremoOpuesto() const;
	/* Bloquea al elemento: no se puede mover, redimensionar ni borrar */
	void bloquear();
	/* True si no es movible, rotable ni redimensionable */
	bool estaBloqueado() const;
	/* Devuelve el nombre del elemento en un string */
	std::string getNombre() const;
	/* Dice si es un determinado elemento, pregunta a getNombre */
	bool es(int const nro_elemento) const;
	/* Devuelve el tipo de elemento */
	int getTipo() const;
	/* Devuelve true si la Coordenada pertenece al Elemento */
	virtual int seleccionar(Coordenada) const = 0;
	/* Imprime por stdout la informacion del elemento */
	virtual void imprimir();
	/* Dice si deben aparecer los puntos de enlace */
	bool getMostrarPuntosDeEnlace() const;
	/* Asigna si deben aparecer los puntos de enlace */
	void setMostrarPuntosDeEnlace(const bool);

};

#endif /*ELEMENTO_H_*/
