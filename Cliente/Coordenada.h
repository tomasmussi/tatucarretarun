#ifndef COORDENADA_H_
#define COORDENADA_H_
#include <string>
#include "Angulo.h"
typedef unsigned long ulong;

class Coordenada {
public:
	/* Atributo publico X */
	long x;
	/* Atributo puclico Y */
	long y;
	/* Atributo publico modulo */
	ulong modulo;
	/* Atributo publico angulo */
	long angulo;
	/* Contructor vacio */
	Coordenada();
	/* Contructor con parametros x e y */
	Coordenada(long x, long y);
	/* Contructor con parametros modulo y angulo */
	Coordenada(ulong modulo, long angulo);
	/* Destructor virtual */
	virtual ~Coordenada();
	/* Se modifica en delta */
	void modificarDelta(Coordenada);
	/* Mueve a otra coordenada */
	void modificar(Coordenada);
	/* Setea X, modifica angulo y modulo */
	void setX(long x);
	/* Setea Y, modifica angulo y modulo */
	void setY(long y);
	/* Setea angulo, modifica X e Y */
	void setAngulo(long angulo);
	/* Setea modulo, modifica X e Y */
	void setModulo(ulong modulo);
	/* Imprime por pantalla el valor */
	void imprimir() const;
	/* Operador Suma */
	Coordenada operator+(const Coordenada) const;
	/* Operador Resta */
	Coordenada operator-(const Coordenada) const;
	

private:
	void actualizarModulo();
	void actualizarAngulo();
	void actualizarX();
	void actualizarY();
	
};

#endif /*COORDENADA_H_*/
