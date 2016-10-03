#include "Angulo.h"
#include <math.h>
#include <iostream>
using namespace std;

// hay q hacer bien esta funcion
void Angulo::corregir(long* valor)
{
	long copia = *valor;
	
	if(fabs(*valor) >= 360.0)
	{
		long vueltas = copia / 360.0;
		copia = copia - (vueltas*360.0);
	}
	
	if(*valor < 0) 
		*valor = 360.0 - copia;
	else
		*valor = copia;
}

double Angulo::aRadianes(long grados) {
	return (2.0*PI * (double)grados) / 360.0;
}

long Angulo::aGrados(double radianes) {
	return ((radianes * 360.0) / (2.0*PI));
}

