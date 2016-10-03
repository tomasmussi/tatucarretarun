#include "Masa.h"
using namespace std;

Masa::Masa() :
	ElementoCircular("Circular:Masa", Coordenada(long(100), long(100)), 10, 0) {
}

Masa::Masa(Coordenada posicion, ulong magnitud) :
	ElementoCircular("Circular:Masa", posicion, magnitud, 0) {
}

Masa::~Masa() {
}

