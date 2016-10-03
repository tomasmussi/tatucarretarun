//cronometro.cpp

#include "cronometro.h"
int Cronometro::estaParado() { return parado;}

void Cronometro::iniciar() {
   comienzo=clock();
   parado=0;
}

void Cronometro::detener() {
   final=clock();
   parado=1;
}

long Cronometro::getMilisegundos() {
   if (estaParado()) return ((final-comienzo)*1000.0)/CLOCKS_PER_SEC;
   return ((clock()-comienzo)*1000.0)/CLOCKS_PER_SEC;
}

