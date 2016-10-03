#include "Coordenada.h"
#include <math.h>
#include <iostream>
#include "constantes.h"

using namespace std;

Coordenada::Coordenada() {

}

Coordenada::Coordenada(long x, long y) {	
	this->x = x;
	this->y = y;
	actualizarAngulo();
	actualizarModulo();
}

Coordenada::Coordenada(ulong modulo, long angulo) {
	this->modulo = modulo;
	this->angulo = angulo;
	actualizarX();
	actualizarY();
}

Coordenada::~Coordenada() {
}

void Coordenada::modificarDelta(Coordenada delta) {
	x += delta.x;
	y += delta.y;
}

void Coordenada::modificar(Coordenada nueva) {
	x = nueva.x;
	y = nueva.y;
}

void Coordenada::imprimir() const {
	cout << "< " << this->x << " , " << this->y << 	" > " << endl;
	/*
	<< "Angulo: "
			<< angulo 
			<< " Modulo: " 
			<< modulo << endl;
	*/
}

void Coordenada::setX(long x) {
	this->x = x;
	actualizarAngulo();
	actualizarModulo();
}

void Coordenada::setY(long y) {
	this->y = y;
	actualizarAngulo();
	actualizarModulo();
}

void Coordenada::setAngulo(long angulo) {
	this->angulo = angulo;
	actualizarX();
	actualizarY();
}

void Coordenada::setModulo(ulong modulo) {
	this->modulo = modulo;
	actualizarX();
	actualizarY();
}

Coordenada Coordenada::operator+(const Coordenada c) const {
	Coordenada aux = Coordenada(this->x + c.x, this->y + c.y);
	return aux;
}

Coordenada Coordenada::operator-(const Coordenada c) const {
	Coordenada aux = Coordenada(this->x - c.x, this->y - c.y);
	return aux;
}

void Coordenada::actualizarModulo() {
	modulo = sqrt(pow(x, 2) + pow(y, 2) );
}

void Coordenada::actualizarAngulo() {
	angulo = Angulo::aGrados(atan2(y,x));
	Angulo::corregir(&angulo);
}

void Coordenada::actualizarX() {
	this->x = modulo * cos(Angulo::aRadianes(angulo));
}

void Coordenada::actualizarY() {
	y = modulo * sin(Angulo::aRadianes(angulo));
}

