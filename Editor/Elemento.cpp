#include "Elemento.h"
#include "Angulo.h"
#include "math.h"

using namespace std;

Elemento::Elemento() {
	this->bloqueado = false;
}	

Elemento::Elemento(const std::string & nombre, Coordenada posicion,
		const ulong magnitud, long angulo) {
	this->nombre = nombre;
	this->posicion = posicion;
	this->magnitud = magnitud;
	Angulo::corregir(&angulo);
	this->angulo = angulo;
	this->bloqueado = false;
	this->mostrarEnlaces = false;
}

Elemento::~Elemento() {
}

void Elemento::setMagnitud(const long valor) {
	if(!bloqueado)
		this->magnitud = valor;
}

ulong Elemento::getMagnitud() const {
	return this->magnitud;
}

void Elemento::setPosicion(const Coordenada nuevaPosicion) {
	if(!bloqueado)
		this->posicion = nuevaPosicion;
}

Coordenada Elemento::getPosicion() const {
	return this->posicion;
}

void Elemento::setAngulo(const long angulo) {
	if(!bloqueado)
		this->angulo = angulo;
}

long Elemento::getAngulo() const {
	return this->angulo;
}

Coordenada Elemento::getExtremoOpuesto() const {
	Coordenada extOp;
	extOp.setX(magnitud * cos(Angulo::aRadianes(angulo)) + posicion.x);
	extOp.setY(magnitud * sin(Angulo::aRadianes(angulo)) + posicion.y);
	return extOp;
}

void Elemento::bloquear() {
	bloqueado = true;
}

bool Elemento::estaBloqueado() const {
	return bloqueado;
}

string Elemento::getNombre() const {
	return nombre;
}

bool Elemento::es(const int nro_elemento) const {
	switch(nro_elemento) {
	case MASA: return (nombre.find("Masa")!=string::npos)? true : false;
	case PUNTO_FIJO: return (nombre.find("PuntoFijo")!=string::npos)? true : false;
	case RUEDA: return (nombre.find("Rueda")!=string::npos)? true : false;
	case BARRA: return (nombre.find("Barra")!=string::npos)? true : false;
	case PLATAFORMA: return (nombre.find("Plataforma")!=string::npos)? true : false;
	case CINTA: return (nombre.find("Cinta")!=string::npos)? true : false;
	case SOGA: return (nombre.find("Soga")!=string::npos)? true : false;	
	case COHETE: return (nombre.find("Cohete")!=string::npos)? true : false;
	case CIRCULAR: return (nombre.find("Circular")!=string::npos)? true : false;
	case LONGITUDINAL: return (nombre.find("Longitudinal")!=string::npos)? true : false;
	case ENLAZABLE: return (nombre.find("Enlazable")!=string::npos)? true : false;
	case PUNTO_ENLACE: return (nombre.find("PuntoDeEnlace")!=string::npos)? true : false;
	case ZONA_LLEGADA: return (nombre.find("ZonaLlegada")!=string::npos)? true : false;
	}
	return false;
}

int Elemento::getTipo() const 
{
	string nom = nombre.substr(nombre.find_last_of(":")+1,nombre.length()-nombre.find_last_of(":")-1);
	if (nom.compare("Masa") == 0)
		return MASA;
		
	if (nom.compare("Rueda") == 0)
		return RUEDA;
	
	if (nom.compare("PuntoFijo") == 0)
		return PUNTO_FIJO;
	
	if (nom.compare("Barra") == 0)
		return BARRA;
	
	if (nom.compare("Plataforma") == 0)
		return PLATAFORMA;	

	if (nom.compare("Soga") == 0)
		return SOGA;
	
	if (nom.compare("Cinta") == 0)
		return CINTA;
	
	if (nom.compare("Cohete") == 0)
		return COHETE;
		
	if (nom.compare("ZonaLlegada") == 0) 
		return ZONA_LLEGADA;
	
		
	return NINGUN_ELEMENTO;
	
}

void Elemento::imprimir() {
	cout << "Elemento:          " << getNombre() << endl;
	cout << "Posicion actual:   ";
	this->posicion.imprimir();
	cout << "Magnitud:          " << this->magnitud << endl
			<< "Angulo actual:     " << this->angulo << endl << endl;
}

bool Elemento::getMostrarPuntosDeEnlace() const {
	return mostrarEnlaces;
}

void Elemento::setMostrarPuntosDeEnlace(const bool b) {
	mostrarEnlaces = b;
}

