#ifndef ANGULO_H_
#define ANGULO_H_
#include "constantes.h"

class Angulo
{
public:
	static void corregir(long* valor);
	/* Pasajes */
	static double aRadianes(long grados);
	static long aGrados(double radianes);
};

#endif /*ANGULO_H_*/
